#include "ServerSession.hpp"
#include "MessageInclude.hpp"
#include <sstream>
#include <algorithm>

using namespace Network;

namespace Logic
{
	const float ServerSession::C_TIMEOUT = 1.0f;

	ServerSession::ServerSession(Network::Server* server, const std::string& adminName, Ruleset* ruleset) 
		: Session(ruleset->GetPlayerCount())
		, mServer(server)
		, mRuleset(ruleset)
	{
		mServer->SetEventInterface(this);

		mPlayers[0] = new Player(adminName, C_TEAM_NONE);
		mPlayerClients[0] = C_STATUS_LOCAL;

		for (Slot s = 1; s < mPlayers.size(); ++s)
		{
			mPlayerClients[s] = C_STATUS_OPEN;
		}
	}

	ServerSession::~ServerSession() throw()
	{
		SafeDelete(mServer);
		SafeDelete(mRuleset);
	}

	bool ServerSession::IsLocalPlayerTurn() const
	{
		return mPlayerClients.find(mCurrentPlayer)->second == C_STATUS_LOCAL;
	}

	bool ServerSession::IsLocalPlayer(PlayerID index) const
	{
		return mPlayers[index] != NULL && mPlayerClients.find(index)->second == C_STATUS_LOCAL;
	}

	Session::ClientSlot ServerSession::GetSlotType(PlayerID playerID) const
	{
		if (mPlayers[playerID] != NULL)
		{
			if (mPlayerClients.find(playerID)->second > 0)
				return C_STATUS_REMOTE;
			return mPlayerClients.find(playerID)->second;
		}
		else
		{
			return C_STATUS_OPEN;
		}
	}

	unsigned short ServerSession::GetPort() const
	{
		return mServer->GetPort();
	}

	const Ruleset* ServerSession::GetRuleset() const
	{
		return mRuleset;
	}

	PlayerID ServerSession::GetWinner() const
	{
		if (mGrid.GetLeadingRow().size() >= mRuleset->GetWinningRowLength())
		{
			return mGrid.GetLeadingPlayer();
		}

		return C_PLAYER_NONE;
	}


	void ServerSession::Update(const GameTime& gameTime)
	{
		// Update the server
		mServer->Update();
		
		// Handle all messages
		Network::SlotMessage message;
		while ((message = mServer->PopMessage()).mMessage != NULL)
		{
			// Get the player this message is about
			//PlayerID slot = GetPlayerID(message.mSlot);

			// If we've received a message, update timeout (unless they're marked to be removed)
			if (std::find(mClientsToRemove.begin(), mClientsToRemove.end(), message.mSlot) == mClientsToRemove.end())
			{
				mClientTimeout[message.mSlot] = C_TIMEOUT;
			}

			// Parse the message content
			switch (message.mMessage->ID())
			{
				case Network::C_MESSAGE_CHAT:
				{
					Network::ChatMessage* m = static_cast<Network::ChatMessage*>(message.mMessage);

					HandleChatMessage(m->mSourceID, m->mTargetID, m->mRecipient, m->mMessage);
				} break;

				case Network::C_MESSAGE_JOIN:
				{
					Network::JoinMessage* m = static_cast<Network::JoinMessage*>(message.mMessage);
				
					HandleJoinMessage(message.mSlot, m->mName);
				} break;
				
				case Network::C_MESSAGE_LEAVE_GAME:
				{
					Network::LeaveGameMessage* m = static_cast<Network::LeaveGameMessage*>(message.mMessage);

					// Do we need this? Notice this on timeout instead?
				} break;

				case Network::C_MESSAGE_PLACE_PIECE:
				{
					Network::PlacePieceMessage* m = static_cast<Network::PlacePieceMessage*>(message.mMessage);

					// If the move is valid, add the marker and move to the next player
					if (mGrid.GetMarkerInCell(m->mX, m->mY) == C_PLAYER_NONE)
					{
						if (mCurrentPlayer == m->mPlayerID)
						{
							mGrid.AddMarker(Logic::Cell(m->mX, m->mY), m->mPlayerID);
							if (mSessionNotifiee != NULL)
								mSessionNotifiee->PlacePiece(m->mPlayerID, Logic::Cell(m->mX, m->mY));

							mServer->Send(Network::PlacePieceMessage(m->mPlayerID, m->mX, m->mY, -1));

							if (!CheckAndHandleWin())
							{
								mCurrentPlayer = mRuleset->GetNextPlayer(mCurrentPlayer);
							}
						}
					}
					
					mServer->Send(Network::TurnMessage(mCurrentPlayer));
				} break;

				case Network::C_MESSAGE_SET_TEAM:
				{
					Network::SetTeamMessage* m = static_cast<Network::SetTeamMessage*>(message.mMessage);

					assert(mPlayers[m->mPlayerID] != NULL);
					assert(m->mTeam >= C_TEAM_NONE);
					assert(m->mTeam < 2);

					if (GetPlayerSlot(message.mSlot) == m->mPlayerID)
					{
						mPlayers[m->mPlayerID]->SetTeam(m->mTeam);

						if (mSessionNotifiee != NULL)
							mSessionNotifiee->SetTeam(m->mPlayerID, m->mTeam);

						mServer->Send(Network::SetTeamMessage(m->mPlayerID, m->mTeam));
					}
				} break;
			}

			SafeDelete(message.mMessage);
		}

		// Decrease timeout
		float dt = gameTime.GetTimeSinceLastTick().Seconds;
		for (TimeoutMap::iterator it = mClientTimeout.begin(); it != mClientTimeout.end(); it++)
		{
			it->second -= dt;
			if (it->second <= 0.0f && mServer->IsConnected(it->first))
			{
				// TODO: Enable timeout again
				//mServer->DisconnectClient(it->first);
			}
		}
	}

	void ServerSession::SendChatMessage(const std::string& message, PlayerID targetID, Network::Recipient::Recipient recipient)
	{
		switch (recipient)
		{
			case Network::Recipient::Broadcast:
				mServer->Send(Network::ChatMessage(0, targetID, recipient, message));
			break;

			case Network::Recipient::Private:
				if (mPlayers[targetID] != NULL && mPlayerClients[targetID] != C_STATUS_LOCAL)
					mServer->Send(mPlayerClients[targetID], Network::ChatMessage(0, targetID, recipient, message));
			break;

			case Network::Recipient::Team:
				for (PlayerID s = 0; s < mPlayers.size(); ++s)
				{
					if (mPlayers[s] != NULL && mPlayers[s]->GetTeam() == mPlayers[0]->GetTeam() && mPlayerClients[s] != C_STATUS_LOCAL)
					{
						mServer->Send(mPlayerClients[s], Network::ChatMessage(0, s, recipient, message));
					}
				}
			break;
		}
	}

	void ServerSession::SendPlacePieceMessage(const Logic::Cell& cell)
	{
		if (mGrid.GetMarkerInCell(cell) == C_PLAYER_NONE)
		{
			assert(mPlayerClients[mCurrentPlayer] == C_STATUS_LOCAL);

			mGrid.AddMarker(cell, mCurrentPlayer);
			mServer->Send(Network::PlacePieceMessage(mCurrentPlayer, cell.x, cell.y, -1));

			if (mSessionNotifiee != NULL)
				mSessionNotifiee->PlacePiece(mCurrentPlayer, cell);
			
			if (!CheckAndHandleWin())
			{
				mCurrentPlayer = mRuleset->GetNextPlayer(mCurrentPlayer);
			}

			mServer->Send(Network::TurnMessage(mCurrentPlayer));
		}
	}

	void ServerSession::SendSetTeamMessage(PlayerID player, Team team)
	{
		if (mPlayers[player] != NULL)
		{
			mPlayers[player]->SetTeam(team);
			mServer->Send(Network::SetTeamMessage(player, team));
		}
	}

	void ServerSession::SendStartMessage()
	{
		mCurrentPlayer = mRuleset->GetStartingPlayer(this);

		mServer->Send(Network::StartGameMessage());
		mServer->Send(Network::TurnMessage(mCurrentPlayer));

		mServer->ShutdownListenSocket();
	}

	void ServerSession::SetLocalPlayerTeam(PlayerID playerID, Team team)
	{
		if (mPlayerClients[playerID] == C_STATUS_LOCAL)
		{
			mPlayers[playerID]->SetTeam(team);
			mServer->Send(Network::SetTeamMessage(playerID, team));

			if (mSessionNotifiee != NULL)
				mSessionNotifiee->SetTeam(playerID, team);
		}
	}

	void ServerSession::BootPlayer(PlayerID playerID)
	{
		if (mPlayers[playerID] != NULL && mPlayerClients[playerID] != C_STATUS_LOCAL)
		{
			mServer->DisconnectClient(mPlayerClients[playerID]); // Own'd
			
			SafeDelete(mPlayers[playerID]);
			mPlayerClients[playerID] = C_STATUS_OPEN;
		}
	}

	void ServerSession::SetSlotType(PlayerID playerSlot, ClientSlot status)
	{
		assert(status == C_STATUS_LOCAL || status == C_STATUS_OPEN);

		BootPlayer(playerSlot);
		mPlayerClients[playerSlot] = status;
	}

	void ServerSession::ClientConnected(Network::Slot slot)
	{
		mClientTimeout[slot] = C_TIMEOUT;
		if (GetPlayerSlot(C_STATUS_OPEN) == C_PLAYER_NONE)
		{
			// Refuse the player, since we have too many players
			mServer->Send(slot, RefuseMessage(RefuseReason::TooManyPlayers));
			mClientsToRemove.push_back(slot);
		}
		else
		{
			// Add client as pending client
			mPendingClients.push_back(slot);
		}
	}

	void ServerSession::ClientDisconnected(Network::Slot slot)
	{
		PlayerID playerSlot = GetPlayerSlot(slot);
		if (playerSlot != C_PLAYER_NONE)
		{
			// TODO: Think about boot
			// TODO: Don't think about boot

			// TODO: Think about timeout?
			// TODO: Should we think about timeout?
			// TODO: Don't think about timeout.

			// TODO: Forget about boot.
			// TODO: Forget about timeout (keep it in mind, though)

			// REDO: Think about timeout.
			// TODO: Ponder meaning of life.

			// TODO: Give up.

			Network::RemovePlayerReason::RemovePlayerReason reason = Network::RemovePlayerReason::Left;
			for (unsigned int i = 0; i < mPlayers.size(); ++i)
			{
				if (mPlayers[i] != NULL && mPlayerClients[i] >= 0 && mServer->IsConnected(mPlayerClients[i]))
				{
					mServer->Send(mPlayerClients[i], Network::RemovePlayerMessage(playerSlot, reason));
				}
			}

			if (mSessionNotifiee != NULL)
				mSessionNotifiee->PlayerDisconnected(playerSlot, mPlayers[playerSlot]->GetName(), reason);

			SafeDelete(mPlayers[playerSlot]);	// Delete player? More proper to make it into an open slot?
			mPlayerClients[playerSlot] = C_STATUS_OPEN;
		}
		else
		{
			std::vector<ClientSlot>::iterator it;

			it = std::find(mPendingClients.begin(), mPendingClients.end(), slot);
			if (it != mPendingClients.end())
				mPendingClients.erase(it);

			it = std::find(mClientsToRemove.begin(), mClientsToRemove.end(), slot);
			if (it != mClientsToRemove.end())
				mClientsToRemove.erase(it);
		}
	}

	PlayerID ServerSession::GetPlayerSlot(ClientSlot slot) const
	{
		for (SlotMap::const_iterator it = mPlayerClients.begin(); it != mPlayerClients.end(); ++it)
		{
			if (it->second == slot)
				return it->first;
		}

		return C_PLAYER_NONE;
	}

	void ServerSession::HandleJoinMessage(Network::Slot clientSlot, const std::string& name)
	{
		bool playerValid = true;
		RefuseReason::RefuseReason reason;

		// Find an open slot
		PlayerID openPlayerID = GetPlayerSlot(C_STATUS_OPEN);
		if (openPlayerID == C_PLAYER_NONE)
		{
			// There are no open slots
			playerValid = false;
			reason = RefuseReason::TooManyPlayers;
		}
		
		// If an open slot is found, check the player's name
		if (playerValid)
		{
			for (PlayerID i = 0; i < mPlayers.size(); ++i)
			{
				if (mPlayers[i] != NULL && mPlayers[i]->GetName() == name)
				{
					playerValid = false;
					reason = RefuseReason::InvalidName;
					break;
				}
			}
		}		

		// If the player is valid, add them to the open slot
		if (playerValid)
		{
			mServer->Send(clientSlot, AcceptMessage(mPlayers.size(), openPlayerID));
			
			mPlayers[openPlayerID] = new Player(name, C_TEAM_NONE);
			mPlayerClients[openPlayerID] = clientSlot;

			for (PlayerID i = 0; i < mPlayers.size(); ++i)
			{
				if (mPlayers[i] != NULL)
				{
					if (i != openPlayerID)
					{
						mServer->Send(clientSlot, AddPlayerMessage(i, mPlayers[i]->GetTeam(), mPlayers[i]->GetName()));
					
						if (mPlayerClients[i] >= 0)
						{
							mServer->Send(mPlayerClients[i], Network::AddPlayerMessage(openPlayerID, mPlayers[openPlayerID]->GetTeam(), mPlayers[openPlayerID]->GetName()));
						}
					}
				}
			}

			if (mSessionNotifiee != NULL)
				mSessionNotifiee->PlayerConnected(openPlayerID);
		}
		else
		{
			// Refuse the player
			mClientsToRemove.push_back(clientSlot);
			mServer->Send(clientSlot, RefuseMessage(reason));
		}

		std::vector<ClientSlot>::iterator it = std::find(mPendingClients.begin(), mPendingClients.end(), clientSlot);
		if (it != mPendingClients.end())
			mPendingClients.erase(it);
	}

	void ServerSession::HandleChatMessage(PlayerID sourceID, PlayerID targetID, Network::Recipient::Recipient recipient, const std::string& message)
	{
		switch (recipient)
		{
			case Network::Recipient::Broadcast:
			{
				for (PlayerID s = 0; s < mPlayers.size(); ++s)
				{
					if (mPlayers[s] != NULL && mPlayerClients[s] != C_STATUS_LOCAL && s != sourceID)
					{
						mServer->Send(mPlayerClients[s], Network::ChatMessage(sourceID, targetID, recipient, message));
					}
				}

				if (mSessionNotifiee != NULL)
				{
					mSessionNotifiee->ReceiveChatMessage(message, recipient, sourceID);
				}
			} break;

			case Network::Recipient::Team:
			{
				Team team = mPlayers[sourceID]->GetTeam();
				for (PlayerID s = 0; s < mPlayers.size(); ++s)
				{
					if (mPlayers[s] != NULL && mPlayerClients[s] != C_STATUS_LOCAL && s != sourceID && mPlayers[s]->GetTeam() == team)
					{
						mServer->Send(mPlayerClients[s], Network::ChatMessage(sourceID, targetID, recipient, message));
					}

					if (mPlayerClients[s] == C_STATUS_LOCAL && mPlayers[s]->GetTeam() == team)
					{
						mSessionNotifiee->ReceiveChatMessage(message, recipient, sourceID);
					}
				}
			} break;

			case Network::Recipient::Private:
			{
				if (mPlayers[targetID] != NULL && mPlayerClients[targetID] != C_STATUS_LOCAL && targetID != sourceID)
				{
					mServer->Send(mPlayerClients[targetID], Network::ChatMessage(sourceID, targetID, recipient, message));
				}

				if (mPlayerClients[targetID] == C_STATUS_LOCAL)
				{
					mSessionNotifiee->ReceiveChatMessage(message, recipient, sourceID);
				}
			} break;
		}
	}

	bool ServerSession::CheckAndHandleWin()
	{
		if (mWinner == C_PLAYER_NONE)
		{
			if (mGrid.GetLeadingRow().size() >= mRuleset->GetWinningRowLength())
			{
				mWinner = mGrid.GetLeadingPlayer();
				mServer->Send(Network::GameOverMessage(mWinner));

				if (mSessionNotifiee != NULL)
					mSessionNotifiee->GameOver(mWinner);
			}
		}

		return mWinner != C_PLAYER_NONE;
	}
}

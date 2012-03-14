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

		mPlayers[0] = new Player(adminName, 0, 0);
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

	unsigned short ServerSession::GetPort() const
	{
		return mServer->GetPort();
	}

	const Ruleset* ServerSession::GetRuleset() const
	{
		return mRuleset;
	}

	std::string ServerSession::GetPlayerName(unsigned int playerSlot) const
	{
		if (mPlayers[playerSlot] == NULL)
			return "";
		return mPlayers[playerSlot]->GetName();
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
			//PlayerSlot slot = GetPlayerSlot(message.mSlot);

			// If we've received a message, update timeout (unless they're marked to be removed)
			/*
			if (std::find(mClientsToRemove.begin(), mClientsToRemove.end(), message.mSlot) == mClientsToRemove.end())
			{
				mTimeoutCounters[slot] = C_TIMEOUT;
			}
			*/

			// Parse the message content
			switch (message.mMessage->ID())
			{
				case Network::C_MESSAGE_CHAT:
				{
					Network::ChatMessage* m = static_cast<Network::ChatMessage*>(message.mMessage);

					switch (m->mRecipient)
					{
						case Network::Recipient::Broadcast:
							for (PlayerSlot s = 0; s < mPlayers.size(); ++s)
							{
								if (mPlayers[s] != NULL && mPlayerClients[s] != C_STATUS_LOCAL && s != m->mSourceID)
								{
									mServer->Send(mPlayerClients[s], Network::ChatMessage(m->mSourceID, m->mTargetID, m->mRecipient, m->mMessage));
								}
							}

							if (mChatReceiver != NULL)
							{
								mChatReceiver->ReceiveChatMessage(m->mMessage, m->mSourceID);
							}
							break;
						case Network::Recipient::Team:
							break;
						case Network::Recipient::Private:
							break;
					}
				} break;

				case Network::C_MESSAGE_JOIN:
				{
					Network::JoinMessage* m = static_cast<Network::JoinMessage*>(message.mMessage);
				
					HandleJoinMessage(message.mSlot, m->mName);
				} break;
				
				case Network::C_MESSAGE_LEAVE_GAME:
				{
					Network::LeaveGameMessage* m = static_cast<Network::LeaveGameMessage*>(message.mMessage);
				} break;

				case Network::C_MESSAGE_PLACE_PIECE:
				{
					Network::PlacePieceMessage* m = static_cast<Network::PlacePieceMessage*>(message.mMessage);
				} break;

				case Network::C_MESSAGE_SET_MARKER:
				{
					Network::SetMarkerMessage* m = static_cast<Network::SetMarkerMessage*>(message.mMessage);
				} break;

				case Network::C_MESSAGE_SET_TEAM:
				{
					Network::SetTeamMessage* m = static_cast<Network::SetTeamMessage*>(message.mMessage);
				} break;
			}

			SafeDelete(message.mMessage);
		}

		// Decrease timeout
		/*
		float dt = gameTime.GetTimeSinceLastTick().Seconds;
		for (TimeoutMap::iterator it = mTimeoutCounters.begin(); it != mTimeoutCounters.end(); ++it)
		{
			it->second -= dt;
			if (it->second <= 0.0f)
			{
				mServer->DisconnectClient(mSlotMap[it->first]);

			}
		}
		*/
	}

	void ServerSession::SendChatMessage(const std::string& message, int targetID, Network::Recipient::Recipient recipient)
	{
		mServer->Send(Network::ChatMessage(0, targetID, recipient, message));
	}

	void ServerSession::ClientConnected(Network::Slot slot)
	{
		//mTimeoutCounters[slot] = C_TIMEOUT;
		if (GetPlayerSlot(C_STATUS_OPEN) == C_INVALID_PLAYER)
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
		PlayerSlot playerSlot = GetPlayerSlot(slot);
		if (playerSlot != C_INVALID_PLAYER)
		{
			SafeDelete(mPlayers[playerSlot]);
			mPlayerClients[playerSlot] = C_STATUS_OPEN;

			// TODO: Think about boot
			for (unsigned int i = 0; i < mPlayers.size(); ++i)
			{
				if (mPlayers[i] != NULL && mPlayerClients[i] >= 0 && mPlayers[i] >= 0)
				{
					mServer->Send(mPlayerClients[i], Network::RemovePlayerMessage(playerSlot, Network::RemovePlayerReason::Left));
				}
			}
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

	ServerSession::PlayerSlot ServerSession::GetPlayerSlot(ClientSlot slot) const
	{
		for (SlotMap::const_iterator it = mPlayerClients.begin(); it != mPlayerClients.end(); ++it)
		{
			if (it->second == slot)
				return it->first;
		}

		return C_INVALID_PLAYER;
	}

	void ServerSession::HandleJoinMessage(Network::Slot clientSlot, const std::string& name)
	{
		bool playerValid = true;
		RefuseReason::RefuseReason reason;

		// Find an open slot
		PlayerSlot openPlayerSlot = GetPlayerSlot(C_STATUS_OPEN);
		if (openPlayerSlot == C_INVALID_PLAYER)
		{
			// There are no open slots
			playerValid = false;
			reason = RefuseReason::TooManyPlayers;
		}
		
		// If an open slot is found, check the player's name
		if (playerValid)
		{
			for (PlayerSlot i = 0; i < mPlayers.size(); ++i)
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
			mServer->Send(clientSlot, AcceptMessage(mPlayers.size(), openPlayerSlot));
			
			mPlayers[openPlayerSlot] = new Player(name, 0, 0);	// TODO: Set a valid team/marker here
			mPlayerClients[openPlayerSlot] = clientSlot;

			for (PlayerSlot i = 0; i < mPlayers.size(); ++i)
			{
				if (mPlayers[i] != NULL)
				{
					if (i != openPlayerSlot)
					{
						mServer->Send(clientSlot, AddPlayerMessage(i, mPlayers[i]->GetTeam(), mPlayers[i]->GetMarkerType(), mPlayers[i]->GetName()));
					
						if (mPlayerClients[i] >= 0)
						{
							mServer->Send(mPlayerClients[i], Network::AddPlayerMessage(openPlayerSlot, mPlayers[openPlayerSlot]->GetTeam(), mPlayers[openPlayerSlot]->GetMarkerType(), mPlayers[openPlayerSlot]->GetName()));
						}
					}
				}
			}
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
}

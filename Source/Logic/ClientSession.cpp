#include "ClientSession.hpp"
#include "MessageInclude.hpp"
#include <cassert>

namespace Logic
{
	const float ClientSession::C_KEEP_ALIVE_DELAY = 0.1f;

	ClientSession::ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID)
		: Session(playerCount)
		, mClientNotifiee(NULL)
		, mClient(client)
		, mSelfID(selfID)
		, mKeepAliveCounter(0.0f)
		, mClientTurn(false)
	{
		mPlayers[selfID] = new Player(playerName, 0);
	}

	ClientSession::~ClientSession()
	{
		SafeDelete(mClient);
	}

	bool ClientSession::IsLocalPlayerTurn() const
	{
		return mCurrentPlayer == mSelfID;
	}

	bool ClientSession::IsLocalPlayer(PlayerID index) const
	{
		return index == mSelfID;
	}

	bool ClientSession::IsConnected() const
	{
		return (mClient != NULL && mClient->IsConnected());
	}

	unsigned int ClientSession::GetSelfID() const
	{
		return mSelfID;
	}

	Session::ClientSlot ClientSession::GetSlotType(PlayerID playerID) const
	{
		if (playerID == mSelfID)
			return C_STATUS_LOCAL;
		return C_STATUS_REMOTE;
	}

	void ClientSession::SetClientNotifiee(ClientNotificationInterface* notifiee)
	{
		mClientNotifiee = notifiee;
	}

	const ClientNotificationInterface* ClientSession::GetClientNotifiee() const
	{
		return mClientNotifiee;
	}

	void ClientSession::Update(const GameTime& gameTime)
	{
		mClient->Update();

		for (int i = mClient->GetQueuedMessageCount() - 1; i >= 0; --i)
		{
			switch (mClient->PeekMessageID(i))
			{
				case Network::C_MESSAGE_CHAT:
				{
					Network::ChatMessage* m = static_cast<Network::ChatMessage*>(mClient->PopMessage(i));

					if (mSessionNotifiee != NULL)
						mSessionNotifiee->ReceiveChatMessage(m->mMessage, m->mRecipient, m->mSourceID);

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_ADD_PLAYER:
				{
					Network::AddPlayerMessage* m = static_cast<Network::AddPlayerMessage*>(mClient->PopMessage(i));
					

					SafeDelete(mPlayers[m->mPlayerID]);
					mPlayers[m->mPlayerID] = new Player(m->mName, m->mTeam);

					if (mSessionNotifiee != NULL)
					{
						mSessionNotifiee->PlayerConnected(m->mPlayerID);
						mSessionNotifiee->SetTeam(m->mPlayerID, m->mTeam);
					}

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_REMOVE_PLAYER:
				{
					Network::RemovePlayerMessage* m = static_cast<Network::RemovePlayerMessage*>(mClient->PopMessage(i));
					
					if (mPlayers[m->mPlayerID] != NULL)
					{
						std::string name = mPlayers[m->mPlayerID]->GetName();
						if (mSessionNotifiee != NULL)
							mSessionNotifiee->PlayerDisconnected(m->mPlayerID, name, m->mReason);

						SafeDelete(mPlayers[m->mPlayerID]);
					}
					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_SET_TEAM:
				{
					Network::SetTeamMessage* m = static_cast<Network::SetTeamMessage*>(mClient->PopMessage(i));

					if (mPlayers[m->mPlayerID] != NULL)
					{
						mPlayers[m->mPlayerID]->SetTeam(m->mTeam);

						if (mSessionNotifiee != NULL)
							mSessionNotifiee->SetTeam(m->mPlayerID, m->mTeam);
					}

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_PLACE_PIECE:
				{
					Network::PlacePieceMessage* m = static_cast<Network::PlacePieceMessage*>(mClient->PopMessage(i));

					mGrid.AddMarker(Logic::Cell(m->mX, m->mY), m->mPlayerID);
					if (mSessionNotifiee != NULL)
						mSessionNotifiee->PlacePiece(m->mPlayerID, Logic::Cell(m->mX, m->mY));

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_REMOVE_PIECE:
				{
					Network::RemovePieceMessage* m = static_cast<Network::RemovePieceMessage*>(mClient->PopMessage(i));

					mGrid.RemoveMarker(Logic::Cell(m->mX, m->mY));

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_TURN:
				{
					Network::TurnMessage* m = static_cast<Network::TurnMessage*>(mClient->PopMessage(i));

					mCurrentPlayer = m->mPlayerID;
					if (m->mPlayerID == mSelfID)
						mClientTurn = true;

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_START_GAME:
				{
					Network::StartGameMessage* m = static_cast<Network::StartGameMessage*>(mClient->PopMessage(i));

					if (mClientNotifiee)
						mClientNotifiee->GameStarted();

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_GAME_OVER:
				{
					Network::GameOverMessage* m = static_cast<Network::GameOverMessage*>(mClient->PopMessage(i));

					mWinner = m->mWinnerID;
					if (mSessionNotifiee != NULL)
						mSessionNotifiee->GameOver(mWinner);
				} break;

				case Network::C_MESSAGE_HIGHLIGHT:
				{
					Network::HighlightMessage* m = static_cast<Network::HighlightMessage*>(mClient->PopMessage(i));

					if (mSessionNotifiee != NULL)
						mSessionNotifiee->SetHighlightedCell(Logic::Cell(m->mX, m->mY), m->mType);
				} break;
			}
		}

		float dt = gameTime.GetTimeSinceLastTick().Seconds;
		mKeepAliveCounter += dt;
		if (mKeepAliveCounter >= C_KEEP_ALIVE_DELAY)
		{
			mClient->Send(Network::StayAliveMessage(mSelfID));
			mKeepAliveCounter = 0.0f;
		}
	}

	void ClientSession::SendChatMessage(const std::string& message, PlayerID targetID, Network::Recipient::Recipient recipient)
	{
		mClient->Send(Network::ChatMessage(mSelfID, targetID, recipient, message));
	}

	void ClientSession::SendPlacePieceMessage(const Logic::Cell& cell)
	{
		if (mGrid.GetMarkerInCell(cell) == C_PLAYER_NONE)
		{
			// Make a sanity check before sending any messages
			if (mCurrentPlayer == mSelfID && mClientTurn)
			{
				mClient->Send(Network::PlacePieceMessage(mSelfID, cell.x, cell.y, -1));
				mClientTurn = false;
			}		
		}
	}

	void ClientSession::SendSetTeamMessage(PlayerID player, Team team)
	{
		if (player == mSelfID)
		{
			mClient->Send(Network::SetTeamMessage(player, team));
		}
	}

	void ClientSession::SendHighlightMessage(const Cell& cell, int highlightType)
	{
		mClient->Send(Network::HighlightMessage(cell.x, cell.y, highlightType));
	}
}
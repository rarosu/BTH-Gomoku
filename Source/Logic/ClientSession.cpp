#include "ClientSession.hpp"
#include "MessageInclude.hpp"
#include <cassert>

namespace Logic
{
	ClientSession::ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID)
		: Session(playerCount)
		, mClient(client)
		, mSelfID(selfID)
	{
		mPlayers[selfID] = new Player(playerName, 0, 0);	// TODO: Get initial team/marker from Accept message
	}

	ClientSession::~ClientSession()
	{
		SafeDelete(mClient);
	}

	void ClientSession::Update()
	{
		mClient->Update();
		

		for (int i = mClient->GetQueuedMessageCount() - 1; i >= 0; --i)
		{
			switch (mClient->PeekMessageID(i))
			{
				case Network::C_MESSAGE_CHAT:
				{
					Network::ChatMessage* m = static_cast<Network::ChatMessage*>(mClient->PopMessage(i));

					if (mChatReceiver != NULL)
					{
						mChatReceiver->ReceiveChatMessage(m->mMessage, m->mSourceID);
					}

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_ADD_PLAYER:
				{
					Network::AddPlayerMessage* m = static_cast<Network::AddPlayerMessage*>(mClient->PopMessage(i));
					
					assert(mPlayers[m->mPlayerID] == NULL);
					mPlayers[m->mPlayerID] = new Player(m->mName, m->mTeam, m->mMarkerID);

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_REMOVE_PLAYER:
				{
					Network::RemovePlayerMessage* m = static_cast<Network::RemovePlayerMessage*>(mClient->PopMessage(i));

					assert(mPlayers[m->mPlayerID] != NULL);
					SafeDelete(mPlayers[m->mPlayerID]);

					switch (m->mReason)
					{
						case Network::RemovePlayerReason::Boot:
							// Report boot in chat
						break;

						case Network::RemovePlayerReason::Left:
							// Report leaving player in chat
						break;

						case Network::RemovePlayerReason::TimeOut:
							// Report timeout in chat
						break;
					}

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_SET_TEAM:
				{
					Network::SetTeamMessage* m = static_cast<Network::SetTeamMessage*>(mClient->PopMessage(i));

					assert(mPlayers[m->mPlayerID] != NULL);
					mPlayers[m->mPlayerID]->SetTeam(m->mTeam);

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_SET_MARKER:
				{
					Network::SetMarkerMessage* m = static_cast<Network::SetMarkerMessage*>(mClient->PopMessage(i));

					assert(mPlayers[m->mPlayerID] != NULL);
					mPlayers[m->mPlayerID]->SetMarkerType(m->mMarkerID);

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_PLACE_PIECE:
				{
					Network::PlacePieceMessage* m = static_cast<Network::PlacePieceMessage*>(mClient->PopMessage(i));

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_REMOVE_PIECE:
				{
					Network::RemovePieceMessage* m = static_cast<Network::RemovePieceMessage*>(mClient->PopMessage(i));

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_TURN:
				{
					Network::TurnMessage* m = static_cast<Network::TurnMessage*>(mClient->PopMessage(i));

					SafeDelete(m);
				} break;

				case Network::C_MESSAGE_START_GAME:
				{
					Network::StartGameMessage* m = static_cast<Network::StartGameMessage*>(mClient->PopMessage(i));

					SafeDelete(m);
				} break;
			}
		}
	}

	void ClientSession::SendChatMessage(const std::string& message, int targetID, Network::Recipient::Recipient recipient)
	{
		mClient->Send(Network::ChatMessage(mSelfID, targetID, recipient, message));
	}

	unsigned int ClientSession::GetPlayerCount() const
	{
		return mPlayers.size();
	}

	std::string ClientSession::GetPlayerName(Network::Slot slot) const
	{
		if (mPlayers[slot] == NULL)
			return "";
		return mPlayers[slot]->GetName();
	}
}
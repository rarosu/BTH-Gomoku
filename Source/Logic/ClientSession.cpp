#include "ClientSession.hpp"
#include "MessageInclude.hpp"
#include <cassert>

namespace Logic
{
	ClientSession::ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID)
		: Session(playerCount)
		, mClient(client)
	{
		mPlayers[selfID] = new Player();
		mPlayers[selfID]->SetName(playerName);
		// TODO: Set mPlayers[selfID] to LocalPlayer
	}

	ClientSession::~ClientSession()
	{
		SafeDelete(mClient);
	}

	void ClientSession::Update()
	{
		mClient->Update();

		Network::Message* message;
		while ( (message = mClient->PopMessage()) != NULL )
		{
			switch (message->ID())
			{
				case Network::C_MESSAGE_CHAT:
				{
					Network::ChatMessage* m = static_cast<Network::ChatMessage*>(message);
				} break;

				case Network::C_MESSAGE_ADD_PLAYER:
				{
					Network::AddPlayerMessage* m = static_cast<Network::AddPlayerMessage*>(message);
					
					assert(mPlayers[m->mPlayerID] == NULL);
					mPlayers[m->mPlayerID] = new Player();
					mPlayers[m->mPlayerID]->SetName(m->mName);
					mPlayers[m->mPlayerID]->SetMarkerType(m->mMarkerID);
					mPlayers[m->mPlayerID]->SetTeam(m->mTeam);
				} break;

				case Network::C_MESSAGE_REMOVE_PLAYER:
				{
					Network::RemovePlayerMessage* m = static_cast<Network::RemovePlayerMessage*>(message);

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
				} break;

				case Network::C_MESSAGE_SET_TEAM:
				{
					Network::SetTeamMessage* m = static_cast<Network::SetTeamMessage*>(message);

					assert(mPlayers[m->mPlayerID] != NULL);
					mPlayers[m->mPlayerID]->SetTeam(m->mTeam);
				} break;

				case Network::C_MESSAGE_SET_MARKER:
				{
					Network::SetMarkerMessage* m = static_cast<Network::SetMarkerMessage*>(message);

					assert(mPlayers[m->mPlayerID] != NULL);
					mPlayers[m->mPlayerID]->SetMarkerType(m->mMarkerID);
				} break;

				case Network::C_MESSAGE_PLACE_PIECE:
				{
					Network::PlacePieceMessage* m = static_cast<Network::PlacePieceMessage*>(message);

					
				} break;

				case Network::C_MESSAGE_REMOVE_PIECE:
				{
					Network::RemovePieceMessage* m = static_cast<Network::RemovePieceMessage*>(message);

				} break;

				case Network::C_MESSAGE_TURN:
				{
					Network::TurnMessage* m = static_cast<Network::TurnMessage*>(message);
				} break;

				case Network::C_MESSAGE_START_GAME:
				{
					Network::StartGameMessage* m = static_cast<Network::StartGameMessage*>(message);
				} break;
			}
		}

		SafeDelete(message);
	}

	unsigned int ClientSession::GetPlayerCount() const
	{
		return mPlayers.size();
	}

	const std::string& ClientSession::GetPlayerName(Network::Slot slot) const
	{
		if (mPlayers[slot] == NULL)
			return "";
		return mPlayers[slot]->GetName();
	}
}
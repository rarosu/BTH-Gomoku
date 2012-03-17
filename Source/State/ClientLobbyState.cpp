#include "ClientLobbyState.hpp"
#include "Console.hpp"
#include <cassert>

namespace State
{
	ClientLobbyState::ClientLobbyState(StateID id, ID3D10Device* device, State::ClientGameState* clientGameState)
		: AbstractLobbyState(id, device)
		, mClientSession(NULL)
		, mClientGameState(clientGameState)
	{}

	void ClientLobbyState::SetSessionArguments(Network::Client* client, int playerCount, int selfID, const std::string& name)
	{
		assert(mClientSession == NULL);

		mClientSession = new Logic::ClientSession(client, name, playerCount, selfID);
		mClientSession->SetClientNotifiee(this);

		SetSession(mClientSession);
	}

	void ClientLobbyState::GameStarted()
	{
		mClientGameState->SetClientSession(mClientSession);
		ChangeState(C_STATE_CLIENT_GAME);
	}

	void ClientLobbyState::AppendComponents()
	{
		mChat->SetName(mClientSession->GetPlayerName(mClientSession->GetSelfID()));
	}
}
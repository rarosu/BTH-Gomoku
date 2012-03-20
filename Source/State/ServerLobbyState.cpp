#include "ServerLobbyState.hpp"
#include <cassert>

namespace State
{
	ServerLobbyState::ServerLobbyState(StateID id, ID3D10Device* device, State::ServerGameState* serverGameState)
		: AbstractLobbyState(id, device),
		mStartButton(NULL),
		mServerSession(NULL),
		mServerGameState(serverGameState)
	{}

	void ServerLobbyState::SetSessionArguments(Network::Server* server, const std::string& adminName, Logic::Ruleset* ruleset)
	{
		assert(server != NULL);
		assert(ruleset != NULL);
		assert(mServerSession == NULL);

		mServerSession = new Logic::ServerSession(server, adminName, ruleset);

		SetSession(mServerSession);
	}

	void ServerLobbyState::AppendStatePopped()
	{
		mServerSession = NULL;
	}

	void ServerLobbyState::AppendComponents()
	{
		int top = mCancelButton->GetPosition().y;
		int right = mCancelButton->GetPosition().x - 20;

		RECT r = { right - mCancelButton->GetWidth(), top, right, top + mCancelButton->GetHeight() };

		mStartButton = new Components::TextButton(mComponents, r);
		mStartButton->Initialize(mDevice, "Start");

		mChat->SetName(mServerSession->GetPlayerName(0));
	}

	void ServerLobbyState::AppendUpdate()
	{
		// Enable start button only if we have enough players
		if (mServerSession->GetPlayerCount() < mServerSession->GetSlotCount())
			mStartButton->SetEnabled(false);
		else
			mStartButton->SetEnabled(true);

		// Check if we can start the game
		if (mStartButton->GetAndResetClickStatus())
		{
			// Start the game!
			mServerGameState->SetServerSession(mServerSession);
			ChangeState(C_STATE_SERVER_GAME);
		}
	}
}
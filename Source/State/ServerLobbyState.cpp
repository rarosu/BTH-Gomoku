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

	void ServerLobbyState::AppendClientConnected(Logic::PlayerID player)
	{
		if (mServerSession->GetSlotCount() == 2)
		{
			mServerSession->SendSetTeamMessage(player, 1);
		}
		else if (mServerSession->GetSlotCount() == 4)
		{
			mTeamMenus[player]->SetCurrentValue(mServerSession->GetPlayerTeam(player));
			mTeamMenus[player]->SetEnabled(true);
		}
	}

	void ServerLobbyState::AppendStatePushed()
	{
		mSlotMenus[0]->SetCurrentValue(0);
		mSlotMenus[0]->SetEnabled(false);
		for (int s = 1; s < mServerSession->GetSlotCount(); ++s)
		{
			mSlotMenus[s]->SetCurrentValue(1);
			mSlotMenus[s]->SetEnabled(true);
		}
		
		mTeamMenus[0]->SetEnabled(true);
		if (mServerSession->GetSlotCount() == 2)
		{
			mTeamMenus[0]->SetCurrentValue(0);
			mTeamMenus[1]->SetCurrentValue(1);

			mTeamMenus[0]->SetEnabled(false);
			mTeamMenus[1]->SetEnabled(false);
		}
	}

	void ServerLobbyState::AppendStatePopped()
	{
		mServerSession = NULL;
		mStartButton = NULL;
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
		mStartButton->SetEnabled(mServerSession->CanStartGame());

		// Check if we can start the game
		if (mStartButton->GetAndResetClickStatus())
		{
			// Start the game!
			mServerGameState->SetServerSession(mServerSession);
			ChangeState(C_STATE_SERVER_GAME);
		}
	}

	void ServerLobbyState::SlotChosen(int playerIndex, int slotIndex)
	{
		switch (slotIndex)
		{
			case 0:
				mServerSession->SetSlotType(playerIndex, Logic::Session::C_STATUS_LOCAL);
			break;

			case 1:
				mServerSession->SetSlotType(playerIndex, Logic::Session::C_STATUS_OPEN);
			break;
		}
	}
}
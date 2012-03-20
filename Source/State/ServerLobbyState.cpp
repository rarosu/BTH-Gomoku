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

	void ServerLobbyState::AppendStatePushed()
	{
		//mServerSession->Set

		mSlotMenu->GetMenuItem(0)->SetCaption(GetCaptionForSlot(Logic::Session::C_STATUS_LOCAL));
		mSlotMenu->GetMenuItem(0)->SetEnabled(false);
		for (int s = 1; s < mServerSession->GetSlotCount(); ++s)
		{
			mSlotMenu->GetMenuItem(s)->SetCaption(GetCaptionForSlot(Logic::Session::C_STATUS_OPEN));
		}
		
		if (mServerSession->GetSlotCount() == 2)
		{
			mTeamMenu->GetMenuItem(0)->SetCaption(GetCaptionForTeam(0));
			mTeamMenu->GetMenuItem(1)->SetCaption(GetCaptionForTeam(1));

			mTeamMenu->GetMenuItem(0)->SetEnabled(false);
			mTeamMenu->GetMenuItem(1)->SetEnabled(false);
		}
		else
		{

		}
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
		mStartButton->SetEnabled(true);

		// Enable start button only if we have enough players
		if (mServerSession->GetPlayerCount() < mServerSession->GetSlotCount())
			mStartButton->SetEnabled(false);

		if (!mServerSession->AreTeamsValid())
			mStartButton->SetEnabled(false);

		static bool hardCodeFlag = true;
		if (hardCodeFlag && mServerSession->GetPlayerCount() == 4)
		{
			mServerSession->SendSetTeamMessage(0, 0);
			mServerSession->SendSetTeamMessage(1, 1);
			mServerSession->SendSetTeamMessage(2, 0);
			mServerSession->SendSetTeamMessage(3, 1);

			hardCodeFlag = false;
		}

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
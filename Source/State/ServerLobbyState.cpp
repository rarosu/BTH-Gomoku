#include "ServerLobbyState.hpp"
#include <cassert>

namespace State
{
	ServerLobbyState::ServerLobbyState(StateID id, ID3D10Device* device)
		: ApplicationState(id),
		  mDevice(device), 
		  mBackground(NULL),
		  mComponents(NULL), 
		  mStartButton(NULL),
		  mCancelButton(NULL),
		  mChat(NULL),
		  mSession(NULL)
	{
		mBackground = new Sprite(mDevice, sViewport, "marbleBG1422x800.png", sViewport->GetWidth(), sViewport->GetHeight());
	}

	ServerLobbyState::~ServerLobbyState() throw()
	{
		SafeDelete(mBackground);
		SafeDelete(mSession);
	}

	void ServerLobbyState::CreateComponents()
	{
		const int C_LABEL_WIDTH = 150;
		const int C_LABEL_HEIGHT = 48;
		const int C_BUTTON_WIDTH = 150;
		const int C_BUTTON_HEIGHT = 48;
		const int C_CHAT_HEIGHT = 150;

		RECT r = {0, 0, 0, 0};
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "ServerLobbyState Group", r);

		r.left = 50;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = 50;
		r.bottom = r.top + C_LABEL_HEIGHT;

		for (unsigned int i = 0; i < mSession->GetRuleset()->GetPlayerCount(); ++i)
		{
			mPlayerLabels.push_back(new Components::Label(mDevice, mComponents, "1.", r, GameFont::Left));

			r.top = r.bottom + C_LABEL_HEIGHT;
			r.bottom = r.top + C_LABEL_HEIGHT;
		}

		r.right = r.left + C_BUTTON_WIDTH;
		r.top += 100;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mStartButton = new Components::TextButton(mComponents, r);
		mStartButton->Initialize(mDevice, "Start");

		r.left += C_BUTTON_WIDTH + 100;
		r.right = r.left + C_BUTTON_WIDTH;
		mCancelButton = new Components::TextButton(mComponents, r);
		mCancelButton->Initialize(mDevice, "Cancel");

		r.left = 0;
		r.right = sViewport->GetWidth();
		r.top = sViewport->GetHeight() - C_CHAT_HEIGHT;
		r.bottom = sViewport->GetHeight();
		mChat = new Components::Console(mDevice, mComponents, r, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
		mChat->SetInputReceiver(this);

		mChat->SetFocus();
		mComponents->SetFocus();
	}

	void ServerLobbyState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		// Check cancel button
		if (mCancelButton->GetAndResetClickStatus())
		{
			ChangeState(C_STATE_MENU);
			SafeDelete(mSession);
			return;
		}

		// Update the session
		mSession->Update(gameTime);

		// Update the player labels
		for (unsigned int i = 0; i < mSession->GetRuleset()->GetPlayerCount(); ++i)
		{
			std::stringstream s;
			s << (i + 1) << ". " << mSession->GetPlayerName(i);

			mPlayerLabels[i]->SetCaption(s.str());
		}
	}

	void ServerLobbyState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0, 0));
	}

	void ServerLobbyState::OnStatePushed()
	{
		assert(mSession != NULL);
		
		CreateComponents();
	}

	void ServerLobbyState::OnStatePopped()
	{
		mSession = NULL;

		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mPlayerLabels.clear();
		mStartButton = NULL;
		mCancelButton = NULL;
		mChat = NULL;
	}

	void ServerLobbyState::SetSessionArguments(Network::Server* server, const std::string& adminName, Logic::Ruleset* ruleset)
	{
		assert(server != NULL);
		assert(ruleset != NULL);
		assert(mSession == NULL);
		mSession = new Logic::ServerSession(server, adminName, ruleset);
	}

	void ServerLobbyState::RecieveInput(std::string input)
	{
		mSession->SendChatMessage(input, -1, Network::Recipient::Broadcast);
	}

	void ServerLobbyState::ReceiveChatMessage(const std::string& message, unsigned int sourceID)
	{
		std::string entry = mSession->GetPlayerName(sourceID) + ": " + message;

		mChat->RecieveInput(entry);
	}
}
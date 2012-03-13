#include "ClientLobbyState.hpp"
#include "Console.hpp"
#include <cassert>
#include <sstream>

namespace State
{
	ClientLobbyState::ClientLobbyState(StateID id, ID3D10Device* device)
		: ApplicationState(id),
		  mDevice(device), mComponents(NULL), mBackground(NULL), mSession(NULL)
	{
		mBackground = new Sprite(mDevice, sViewport, "marbleBG1422x800.png", sViewport->GetWidth(), sViewport->GetHeight());
	}

	ClientLobbyState::~ClientLobbyState() throw()
	{
		SafeDelete(mBackground);
		SafeDelete(mSession);
	}

	void ClientLobbyState::CreateComponents()
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

		for (unsigned int i = 0; i < mSession->GetPlayerCount(); ++i)
		{
			mPlayerLabels.push_back(new Components::Label(mDevice, mComponents, "1.", r, GameFont::Left));

			r.top = r.bottom + C_LABEL_HEIGHT;
			r.bottom = r.top + C_LABEL_HEIGHT;
		}

		r.right = r.left + C_BUTTON_WIDTH;
		r.top += 100;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mCancelButton = new Components::TextButton(mComponents, r);
		mCancelButton->Initialize(mDevice, "Cancel");

		/*
		r.left = 0;
		r.right = sViewport->GetWidth();
		r.top = sViewport->GetHeight() - C_CHAT_HEIGHT;
		r.bottom = sViewport->GetHeight() - 50;
		mChat = new Components::Console(mDevice, mComponents, r, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
		*/

		//mChat->SetFocus();
		mComponents->SetFocus();
	}

	void ClientLobbyState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		// Check if we want to cancel
		if(mCancelButton->GetAndResetClickStatus())
		{
			SafeDelete(mSession);
			ChangeState(C_STATE_MENU);
			
			return;
		}

		// Update the session
		mSession->Update();

		// Update the player labels
		for (unsigned int i = 0; i < mSession->GetPlayerCount(); ++i)
		{
			std::stringstream s;
			s << (i + 1) << ". " << mSession->GetPlayerName(i);

			mPlayerLabels[i]->SetCaption(s.str());
		}
	}

	void ClientLobbyState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0.0f, 0.0f));
	}

	void ClientLobbyState::OnStatePushed()
	{
		assert(mSession != NULL);
		
		CreateComponents();
	}

	void ClientLobbyState::OnStatePopped()
	{
		mSession = NULL;

		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mCancelButton = NULL;
		mPlayerLabels.clear();
	}

	void ClientLobbyState::SetSessionArguments(Network::Client* client, int playerCount, int selfID, const std::string& name)
	{
		assert(mSession == NULL);

		mSession = new Logic::ClientSession(client, name, playerCount, selfID);
	}
}
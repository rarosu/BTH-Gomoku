#include "ServerLobbyState.hpp"
#include "Console.hpp"
#include <cassert>

namespace State
{
	ServerLobbyState::ServerLobbyState(StateID id, ID3D10Device* device)
		: ApplicationState(id),
		  mDevice(device), 
		  mComponents(NULL), 
		  mSession(NULL),
		  mBackground(NULL)
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
		// Create new component group
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "ServerLobbyState Group");

		// Create title label
		int lblCenterX = sViewport->GetWidth() / 2;
		int lblHalfWidth = 300;
		RECT labelTPos = { lblCenterX - lblHalfWidth, 50, lblCenterX + lblHalfWidth, 150 };
		Components::Label* labelTitle = new Components::Label(mDevice, mComponents, "GAME LOBBY", labelTPos);

		// Create player list
		LONG lblTop = 200;
		LONG lblLeft = 0;
		LONG lblHeight = 40;
		LONG lblWidth = 200;

		const std::string playerCaptions[] = { "PLAYERS: ", "1. ", "2. ", "3. ", "4. " };

		for(int h = 0; h < 5; ++h)
		{
			RECT plblPos = { lblLeft, lblTop, lblLeft + lblWidth, lblTop + lblHeight };
			mPlayerLabels.push_back(new Components::Label(mDevice, mComponents, playerCaptions[h], plblPos, GameFont::Left));
			lblTop += lblHeight;
		}

		// Create the team buttons and labels
		const std::string lblCaptions[] = { "Team 1: ", "Team 2: " };
		const std::string btnCaptions[] = { "Team 1: Player 1", "Team 1: Player 2", 
											"Team 2: Player 1", "Team 2: Player 2" };
		LONG btnWidth = 200;
		LONG btnHeight = 40;
		LONG btnLeft = (LONG)sViewport->GetWidth() - (btnWidth + 30);
		LONG btnTop = 200;
		const int padding = 20;

		for(int i = 0; i < 2; ++i)
		{
			RECT lblPos = { btnLeft, btnTop, btnLeft + btnWidth, btnTop + btnHeight };
			Components::Label* labelTitle = new Components::Label(mDevice, mComponents, lblCaptions[i], lblPos);
			btnTop += btnHeight;
			for(int j = 0; j < 2; ++j)
			{
				mButtons.push_back(new Components::TextButton(mComponents));
		
				RECT buttonPos = { btnLeft, btnTop, btnLeft + btnWidth, btnTop + btnHeight };
				mButtons[i * 2 + j]->Initialize(mDevice, buttonPos, btnCaptions[i * 2 + j]);
				btnTop += btnHeight;
			}
			btnTop += padding;
		}

		// Create Start Game- and Cancel-buttons
		mButtons.push_back(new Components::TextButton(mComponents));
		int btnSGLeft = sViewport->GetWidth() / 2 - (192 + 30);
		int btnSGCTop = sViewport->GetHeight() - 250;
		RECT btnSGPos = { btnSGLeft, btnSGCTop, btnSGLeft + btnWidth, btnSGCTop + btnHeight };
		mButtons[mButtons.size() - 1]->Initialize(mDevice, btnSGPos, "Start Game");

		mButtons.push_back(new Components::TextButton(mComponents));
		int btnCLeft = sViewport->GetWidth() / 2 + 30;
		RECT btnCPos = { btnCLeft, btnSGCTop, btnCLeft + btnWidth, btnSGCTop + btnHeight };
		mButtons[mButtons.size() - 1]->Initialize(mDevice, btnCPos, "Cancel");

		// Create chat console
		RECT chatPos = { 0, sViewport->GetHeight() - 180, sViewport->GetWidth(), sViewport->GetHeight() };
		Components::Console* chatWindow = new Components::Console(mDevice, mComponents, chatPos, C_COLOR_WINDOW_BG);

		chatWindow->SetFocus();
		mComponents->SetFocus();
	}

	void ServerLobbyState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		mSession->Update(gameTime);

		if(mButtons[LobbyButton::StartGame]->GetAndResetClickStatus())
			ChangeState(C_STATE_IN_GAME);
		if(mButtons[LobbyButton::Cancel]->GetAndResetClickStatus())
		{
			SafeDelete(mSession);
			ChangeState(C_STATE_MENU);
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
		mButtons.clear();
	}

	void ServerLobbyState::SetSessionArguments(Network::Server* server, const std::string& adminName, Logic::Ruleset* ruleset)
	{
		assert(server != NULL);
		assert(ruleset != NULL);
		assert(mSession == NULL);
		mSession = new Logic::ServerSession(server, adminName, ruleset);
	}
}
#include "ClientLobbyState.hpp"
#include "Console.hpp"
#include <cassert>
#include <sstream>

namespace State
{
	ClientLobbyState::ClientLobbyState(StateID id, ID3D10Device* device)
		: ApplicationState(id),
		  mDevice(device), mComponents(NULL), mBackground(NULL), mClientSession(NULL)
	{
		mBackground = new Sprite(mDevice, sViewport, "marbleBG1422x800.png", sViewport->GetWidth(), sViewport->GetHeight());
	}

	ClientLobbyState::~ClientLobbyState() throw()
	{
		SafeDelete(mBackground);
		SafeDelete(mClientSession);
	}

	void ClientLobbyState::CreateComponents()
	{
		// Create new component group
		const int C_LABEL_WIDTH = 150;
		const int C_BUTTON_WIDTH = 150;

		RECT r = {0, 0, 0, 0};

		mComponents = new Components::ComponentGroup(sRootComponentGroup, "ClientLobbyState Group", r);

		r.left = 100;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = 100;
		r.bottom = r.top + 48;
		Components::Label* labelTitle = new Components::Label(mDevice, mComponents, "GAME LOBBY", r);

		for (Network::Slot i = 0; i < mClientSession->GetPlayerCount(); ++i)
		{
			std::stringstream s;
			s << i << ". " << mClientSession->GetPlayerName(i);

			r.left = 100;
			r.right = r.left + C_LABEL_WIDTH;
			r.top += 100;
			r.bottom = r.top + 48;
			mPlayerLabels.push_back(new Components::Label(mDevice, mComponents, s.str(), r));
		}

		r.left = 100;
		r.right = r.left + C_BUTTON_WIDTH;
		r.top += 100;
		r.bottom = r.top + 48;
		mCancelButton = new Components::TextButton(mComponents, r);
		mCancelButton->Initialize(mDevice, "Cancel");

		mCancelButton->SetFocus();
		mComponents->SetFocus();

		/*LONG lblTop = 200;
		LONG lblLeft = 0;
		LONG lblHeight = 40;
		LONG lblWidth = 200;*/

		// Create new component group
		//mComponents = new Components::ComponentGroup(sRootComponentGroup, "ClientLobbyState Group");

		//mBackground = new Sprite(mDevice, sViewport,  "marbleBG1422x800.png", 
		//					 sViewport->GetWidth(), sViewport->GetHeight());


		// Create the team buttons and labels
		/*const std::string lblCaptions[] = { "Team 1: ", "Team 2: " };
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
				RECT buttonPos = { btnLeft, btnTop, btnLeft + btnWidth, btnTop + btnHeight };
				mButtons.push_back(new Components::TextButton(mComponents, buttonPos));
				mButtons[i * 2 + j]->Initialize(mDevice, btnCaptions[i * 2 + j]);
				btnTop += btnHeight;
			}
			btnTop += padding;
		}

		// Create Start Game- and Cancel-buttons
		int btnSGLeft = sViewport->GetWidth() / 2 - (192 + 30);
		int btnSGCTop = sViewport->GetHeight() - 250;
		RECT btnSGPos = { btnSGLeft, btnSGCTop, btnSGLeft + btnWidth, btnSGCTop + btnHeight };
		mButtons.push_back(new Components::TextButton(mComponents, btnSGPos));
		mButtons[mButtons.size() - 1]->Initialize(mDevice, "Start Game");

		int btnCLeft = sViewport->GetWidth() / 2 + 30;
		RECT btnCPos = { btnCLeft, btnSGCTop, btnCLeft + btnWidth, btnSGCTop + btnHeight };
		mButtons.push_back(new Components::TextButton(mComponents, btnCPos));
		mButtons[mButtons.size() - 1]->Initialize(mDevice, "Cancel");

		// Create chat console
		RECT chatPos = { 0, sViewport->GetHeight() - 180, sViewport->GetWidth(), sViewport->GetHeight() };
		Components::Console* chatWindow = new Components::Console(mDevice, mComponents, chatPos, C_COLOR_WINDOW_BG);
		mComponents->SetFocusedComponent(chatWindow);
		*/

		//// Create title label
		//int lblCenterX = sViewport->GetWidth() / 2;
		//int lblHalfWidth = 300;
		//RECT labelTPos = { lblCenterX - lblHalfWidth, 50, lblCenterX + lblHalfWidth, 150 };
		//

		//// Create player list
		

		//const std::string playerCaptions[] = { "PLAYERS: ", "1. ", "2. ", "3. ", "4. " };

		//for(int h = 0; h < 5; ++h)
		//{
		//	RECT plblPos = { lblLeft, lblTop, lblLeft + lblWidth, lblTop + lblHeight };
		//	mPlayerLabels.push_back(new Components::Label(mDevice, mComponents, playerCaptions[h], plblPos, GameFont::Left));
		//	lblTop += lblHeight;
		//}

		//// Create the team buttons and labels
		//const std::string lblCaptions[] = { "Team 1: ", "Team 2: " };
		//const std::string btnCaptions[] = { "Team 1: Player 1", "Team 1: Player 2", 
		//									"Team 2: Player 1", "Team 2: Player 2" };
		//LONG btnWidth = 200;
		//LONG btnHeight = 40;
		//LONG btnLeft = (LONG)sViewport->GetWidth() - (btnWidth + 30);
		//LONG btnTop = 200;
		//const int padding = 20;

		//for(int i = 0; i < 2; ++i)
		//{
		//	RECT lblPos = { btnLeft, btnTop, btnLeft + btnWidth, btnTop + btnHeight };
		//	Components::Label* labelTitle = new Components::Label(mDevice, mComponents, lblCaptions[i], lblPos);
		//	btnTop += btnHeight;
		//	for(int j = 0; j < 2; ++j)
		//	{
		//		mButtons.push_back(new Components::TextButton(mComponents));
		//
		//		RECT buttonPos = { btnLeft, btnTop, btnLeft + btnWidth, btnTop + btnHeight };
		//		mButtons[i * 2 + j]->Initialize(mDevice, buttonPos, btnCaptions[i * 2 + j]);
		//		btnTop += btnHeight;
		//	}
		//	btnTop += padding;
		//}

		//// Create Start Game- and Cancel-buttons
		//mButtons.push_back(new Components::TextButton(mComponents));
		//int btnSGLeft = sViewport->GetWidth() / 2 - (192 + 30);
		//int btnSGCTop = sViewport->GetHeight() - 250;
		//RECT btnSGPos = { btnSGLeft, btnSGCTop, btnSGLeft + btnWidth, btnSGCTop + btnHeight };
		//mButtons[mButtons.size() - 1]->Initialize(mDevice, btnSGPos, "Start Game");

		//mButtons.push_back(new Components::TextButton(mComponents));
		//int btnCLeft = sViewport->GetWidth() / 2 + 30;
		//RECT btnCPos = { btnCLeft, btnSGCTop, btnCLeft + btnWidth, btnSGCTop + btnHeight };
		//mButtons[mButtons.size() - 1]->Initialize(mDevice, btnCPos, "Cancel");

		//// Create chat console
		//RECT chatPos = { 0, sViewport->GetHeight() - 180, sViewport->GetWidth(), sViewport->GetHeight() };
		//Components::Console* chatWindow = new Components::Console(mDevice, mComponents, chatPos, C_COLOR_WINDOW_BG);
		//mComponents->SetFocusedComponent(chatWindow);

	}

	void ClientLobbyState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		

		/*if(mButtons[LobbyButton::StartGame]->GetAndResetClickStatus())
			ChangeState(C_STATE_IN_GAME);
		if(mButtons[LobbyButton::Cancel]->GetAndResetClickStatus())
		{
			SafeDelete(mClientSession);
			ChangeState(C_STATE_MENU);
		}*/
	}

	void ClientLobbyState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0.0f, 0.0f));
	}

	void ClientLobbyState::OnStatePushed()
	{
		assert(mClientSession != NULL);
		
		CreateComponents();
	}

	void ClientLobbyState::OnStatePopped()
	{
		mClientSession = NULL;

		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mCancelButton = NULL;
		mPlayerLabels.clear();
	}

	void ClientLobbyState::SetSessionArguments(Network::Client* client, int playerCount, int selfID, const std::string& name)
	{
		assert(mClientSession == NULL);

		mClientSession = new Logic::ClientSession(client, name, playerCount, selfID);
	}
}
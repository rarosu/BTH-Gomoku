#include "MenuState.hpp"
#include <sstream>

namespace State
{
	const std::string MenuState::C_MENU_CAPTIONS[] = { "Start Game", "Watch Replay", "Credits", "Exit Game" };
	const std::string MenuState::C_START_GAME_CAPTIONS[] = { "Create Game", "Join Game" };

	MenuState::MenuState(StateID id, ID3D10Device* device) 
		: ApplicationState(id), mDevice(device), mComponents(NULL), mBackground(NULL), mDebugFont(NULL)
	{
		mDebugFont = new GameFont(mDevice);
	}

	MenuState::~MenuState() throw()
	{
		SafeDelete(mBackground);
	}

	void MenuState::CreateComponents()
	{
		RECT compPos = { 0, 0, 0, 0 };
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "MenuState Group", compPos);

		mBackground = new Sprite(mDevice, sViewport,  "titleScreen1422x800.png", 
							 sViewport->GetWidth(), sViewport->GetHeight());

		// Create menu buttons
		const int width = 300;
		const int height = 75;
		const int padding = 0;
		LONG leftOffset = 0;
		LONG topOffset = (LONG)sViewport->GetHeight() - MenuButton::Count * (height + padding) + padding;
		RECT pos = { 0, topOffset, width, topOffset + height};
		mMenuButtons = new Components::ClickMenu(mComponents, mDevice, pos, width, height); 

		for(int i = 0; i < MenuButton::Count; ++i)
		{
			mMenuButtons->AddMenuItem(C_MENU_CAPTIONS[i]);
		}

		for (int i = 0; i < 2; ++i)
		{
			mMenuButtons->GetMenuItem(C_MENU_CAPTIONS[MenuButton::StartGame])->AddSubItem(C_START_GAME_CAPTIONS[i]);
		}

		mMenuButtons->GetMenuItem(C_MENU_CAPTIONS[MenuButton::WatchReplay])->SetEnabled(false);
		mMenuButtons->GetMenuItem(C_MENU_CAPTIONS[MenuButton::Credits])->SetEnabled(false);
		mMenuButtons->GetSubMenu(C_MENU_CAPTIONS[MenuButton::StartGame])->GetMenuItem(C_START_GAME_CAPTIONS[1])->SetEnabled(true);
		
		mComponents->SetFocusedComponent(mMenuButtons->GetSubMenu(C_MENU_CAPTIONS[MenuButton::WatchReplay]));
	}

	void MenuState::OnStatePushed()
	{
		CreateComponents();
		mComponents->SetFocus();
	}

	void MenuState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		SafeDelete(mBackground);
	}

	void MenuState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		// Handle quick exit shortcut
		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE] && !prevInput.Keyboard.keyIsPressed[VK_ESCAPE])
			QuitApplication();

		// Handle the menu events
		if (mMenuButtons->GetSubMenu(C_MENU_CAPTIONS[MenuButton::StartGame])->GetAndResetClickStatus(C_START_GAME_CAPTIONS[0]))
			ChangeState(C_STATE_CREATE_GAME);
		if (mMenuButtons->GetSubMenu(C_MENU_CAPTIONS[MenuButton::StartGame])->GetAndResetClickStatus(C_START_GAME_CAPTIONS[1]))
			ChangeState(C_STATE_JOIN_GAME);
		if (mMenuButtons->GetAndResetClickStatus(C_MENU_CAPTIONS[MenuButton::WatchReplay])) 
			{} // Do nothing here yet
		if (mMenuButtons->GetAndResetClickStatus(C_MENU_CAPTIONS[MenuButton::Credits]))
			{} // Change to CreditsState
		if (mMenuButtons->GetAndResetClickStatus(C_MENU_CAPTIONS[MenuButton::Exit]))
			QuitApplication();
	}

	void MenuState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0, 0));

		POINT pos = { 0, 0 };
		std::stringstream ss;
		ss << "Click Menu Position: (" << mMenuButtons->GetPosition().x << ", ";
		ss << mMenuButtons->GetPosition().y << ")\tIsVisible(): " << mMenuButtons->IsVisible();
		for(int i = 0; i < 4; ++i)
		{
			D3DXVECTOR2 itemPos = mMenuButtons->GetMenuItem(C_MENU_CAPTIONS[i])->GetPosition();
			ss << C_MENU_CAPTIONS[i] << " Position: (" << itemPos.x << ", " << itemPos.y << ")";
		}
		ss << "\n\tStartGame Position: ";
		mDebugFont->WriteText(ss.str(), pos, D3DXCOLOR(0.0, 0.0, 1.0, 1.0));
	}
}
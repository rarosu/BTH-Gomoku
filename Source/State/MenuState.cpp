#include "MenuState.hpp"

namespace State
{
	MenuState::MenuState(StateID id, ID3D10Device* device) 
		: ApplicationState(id), mDevice(device), mComponents(NULL), mBackground(NULL)
	{
	}

	MenuState::~MenuState() throw()
	{
		SafeDelete(mBackground);
	}

	void MenuState::CreateComponents()
	{
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "MenuState Group");

		mBackground = new Sprite(mDevice, sViewport,  "titleScreen1422x800.png", 
							 sViewport->GetWidth(), sViewport->GetHeight());

		// Create menu buttons
		const std::string btnCaptions[] = { "Start Game", "Watch Replay", "Credits", "Exit Game" };
		const int width = 300;
		const int height = 75;
		const int padding = 0;
		LONG leftOffset = 0;
		LONG topOffset = (LONG)sViewport->GetHeight() - MenuButton::Count * (height + padding) + padding;
		POINT pos = { 0, topOffset };
		mMenuButtons = new Components::ClickMenu(mComponents, mDevice, pos, width, height); 

		for(int i = 0; i < MenuButton::Count; ++i)
		{
			mMenuButtons->AddMenuItem(btnCaptions[i]);
		}

		/*mButtons[1]->SetEnabled(false);
		mButtons[2]->SetEnabled(false);*/
	}

	void MenuState::OnStatePushed()
	{
		CreateComponents();
		mComponents->GiveFocus();
	}

	void MenuState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mButtons.clear();
		SafeDelete(mBackground);
	}

	void MenuState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE] && !prevInput.Keyboard.keyIsPressed[VK_ESCAPE])
			QuitApplication();

		//if(mButtons[MenuButton::StartGame]->GetAndResetClickStatus())
		//	ChangeState(C_STATE_CREATE_GAME);
		///*if(mButtons[MenuButton::WatchReplay]->GetAndResetClickStatus())
		//	ChangeState(C_STATE_NETWORK_LOBBY);*/
		///*if(mButtons.at(MenuButton::Options)->GetAndResetClickStatus())
		//	ChangeState(C_STATE_IN_GAME);*/
		//if(mButtons[MenuButton::Exit]->GetAndResetClickStatus())
		//	QuitApplication();

		//mComponents->Update(gameTime, currInput, prevInput);
	}

	void MenuState::Draw()
	{
		mComponents->Draw();
		mBackground->Draw(D3DXVECTOR2(0, 0));
	}
}
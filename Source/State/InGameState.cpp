#include "InGameState.hpp"
#include "Globals.hpp"

namespace State
{
	InGameState::InGameState(StateID id, ID3D10Device* device) :
		ApplicationState(id),
		mDevice(device),
		mGrid(NULL),
		mComponents(NULL),
		mScene(NULL)
	{

	}

	InGameState::~InGameState() throw()
	{
		SafeDelete(mGrid);
		SafeDelete(mScene);
	}

	float CalculateAspectRatio(const Viewport& viewport)
	{
		return static_cast<float>(viewport.GetWidth()) / static_cast<float>(viewport.GetHeight());
	}

	void InGameState::CreateComponents()
	{
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "Ingame group");

		/*RECT menuPos = { 100, 100, 200, 200 };
		mDragonAgeMenu = new Components::Menu(mDevice, mComponents, menuPos);
		mDragonAgeMenu->SetVisible(false);*/

		mGrid = new Logic::Grid();
		mScene = new Scene(mDevice, CalculateAspectRatio(*sViewport));
	}

	void InGameState::OnStatePushed()
	{
		CreateComponents();
		mComponents->GiveFocus();
	}

	void InGameState::OnStatePopped()
	{
		SafeDelete(mGrid);
		SafeDelete(mScene);

		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		//SafeDelete(mDragonAgeMenu);
	}

	void InGameState::OnResize()
	{
		if (mScene != NULL)
			mScene->ResizeFrustum(CalculateAspectRatio(*sViewport));
	}

	void InGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE] && !prevInput.Keyboard.keyIsPressed[VK_ESCAPE])
			ChangeState(C_STATE_MENU);

		mScene->Update(*mGrid, *sViewport, currInput, prevInput, gameTime);

		/*if(currInput.Mouse.buttonIsPressed[C_MOUSE_RIGHT] && !prevInput.Mouse.buttonIsPressed[C_MOUSE_RIGHT])
		{
			mDragonAgeMenu->SetVisible(true);

			RECT menuPos = { currInput.Mouse.x - 50, currInput.Mouse.y - 50, 
							 currInput.Mouse.x + 50, currInput.Mouse.y + 50 };
			mDragonAgeMenu->SetPosition(menuPos);
			mComponents->SetFocus(mDragonAgeMenu);
		}
		else if (!currInput.Mouse.buttonIsPressed[C_MOUSE_RIGHT] && prevInput.Mouse.buttonIsPressed[C_MOUSE_RIGHT])
			mDragonAgeMenu->SetVisible(false);

		if(mDragonAgeMenu->IsVisible())
			mDragonAgeMenu->Update(gameTime, currInput, prevInput);*/
	}

	void InGameState::Draw()
	{
		mScene->Draw();

		/*if(mDragonAgeMenu->IsVisible())
			mDragonAgeMenu->Draw();*/
	}
}
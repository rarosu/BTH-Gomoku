#include "InGameState.hpp"
#include "Globals.hpp"

namespace State
{
	InGameState::InGameState(StateID id, ID3D10Device* device, const Frustrum& viewFrustrum, InputSubscription* inputSubscription) :
		ApplicationState(id),
		mDevice(device),
		mGrid(NULL),
		mScene(NULL),
		mCamera(NULL),
		mMarker(NULL),
		mViewFrustrum(viewFrustrum),
		mInputSubscription(inputSubscription),
		mShowMenu(false)
	{
		RECT menuPos = { 100, 100, 200, 200 };
		mDragonAgeMenu = new Components::Menu(mDevice, sInputManager, menuPos);
	}

	InGameState::~InGameState() throw()
	{
		SafeDelete(mGrid);
		SafeDelete(mScene);
		SafeDelete(mCamera);
		SafeDelete(mMarker);
	}

	void InGameState::OnStatePushed()
	{
		mGrid = new Logic::Grid();
		mScene = new Scene(mDevice, mInputSubscription);
		mCamera = new Camera(D3DXVECTOR3(0, 0, 0), 
							 D3DXVECTOR3(0, -1.0f, 2.0f), 
							 D3DXVECTOR3(0, 1.0f, 0), 
							 mViewFrustrum,
							 mInputSubscription);
		mMarker = new Marker(mDevice, 6, D3DXVECTOR3(5.0f, 1.0f, 5.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}

	void InGameState::OnStatePopped()
	{
		SafeDelete(mGrid);
		SafeDelete(mScene);
		SafeDelete(mCamera);
		SafeDelete(mMarker);
	}

	void InGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE] && !prevInput.Keyboard.keyIsPressed[VK_ESCAPE])
			ChangeState(C_STATE_MENU);

		mCamera->Update(prevInput, currInput, gameTime);
		mScene->Update(*mGrid, *mCamera, *sViewport, currInput);
		mMarker->Update(*mCamera);

		if(currInput.Mouse.buttonIsPressed[C_MOUSE_RIGHT] && !prevInput.Mouse.buttonIsPressed[C_MOUSE_RIGHT])
		{
			mShowMenu = true;
			RECT menuPos = { currInput.Mouse.x - 50, currInput.Mouse.y - 50, 
							 currInput.Mouse.x + 50, currInput.Mouse.y + 50 };
			mDragonAgeMenu->SetPosition(menuPos);
		}
		else if (!currInput.Mouse.buttonIsPressed[C_MOUSE_RIGHT] && prevInput.Mouse.buttonIsPressed[C_MOUSE_RIGHT])
			mShowMenu = false;

		if(mShowMenu)
			mDragonAgeMenu->Update(gameTime, currInput, prevInput);
	}

	void InGameState::Draw()
	{
		mScene->Draw(*mCamera);
		mMarker->Draw();

		if(mShowMenu)
			mDragonAgeMenu->Draw();
	}

	void InGameState::SetViewFrustrum(const Frustrum& viewFrustrum)
	{
		mViewFrustrum = viewFrustrum;

		// If we're already in this state, we need to update the projection matrix
		if (mCamera != NULL)
		{
			mCamera->CreateProjectionMatrix(mViewFrustrum);
		}
	}
}
#include "InGameState.hpp"
#include "Globals.hpp"

namespace State
{
	InGameState::InGameState(StateID id, ID3D10Device* device) :
		ApplicationState(id),
		mDevice(device),
		mGrid(NULL),
		mScene(NULL),
		mCamera(NULL),
		mMarkerBlue(NULL),
		mMarkerGreen(NULL),
		mShowMenu(false)
	{
		mViewFrustum.nearDistance = 1.0f;
		mViewFrustum.farDistance = 1000.0f;
		mViewFrustum.fovY = (float)D3DX_PI * 0.25f;
		mViewFrustum.aspectRatio = static_cast<float>(sViewport->GetWidth()) / static_cast<float>(sViewport->GetHeight());

		RECT menuPos = { 100, 100, 200, 200 };
		mDragonAgeMenu = new Components::Menu(mDevice, sInputManager, menuPos);
	}

	InGameState::~InGameState() throw()
	{
		SafeDelete(mGrid);
		SafeDelete(mScene);
		SafeDelete(mCamera);
		SafeDelete(mMarkerBlue);
	}

	void InGameState::OnStatePushed()
	{
		mGrid = new Logic::Grid();
		mScene = new Scene(mDevice, sInputManager);
		mCamera = new Camera(D3DXVECTOR3(0, 0, 0), 
							 D3DXVECTOR3(0, -1.0f, 2.0f), 
							 D3DXVECTOR3(0, 1.0f, 0), 
							 mViewFrustum,
							 sInputManager);
		mMarkerBlue = new Marker(mDevice, 6, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		mMarkerGreen = new Marker(mDevice, 6, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

		mCamera->CreateProjectionMatrix(mViewFrustum);
	}

	void InGameState::OnStatePopped()
	{
		SafeDelete(mGrid);
		SafeDelete(mScene);
		SafeDelete(mCamera);
		SafeDelete(mMarkerBlue);
		SafeDelete(mMarkerGreen);
	}

	void InGameState::OnResize()
	{
		mViewFrustum.aspectRatio = static_cast<float>(sViewport->GetWidth()) / static_cast<float>(sViewport->GetHeight());

		if (mCamera != NULL)
			mCamera->CreateProjectionMatrix(mViewFrustum);
	}

	void InGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE] && !prevInput.Keyboard.keyIsPressed[VK_ESCAPE])
			ChangeState(C_STATE_MENU);

		mCamera->Update(prevInput, currInput, gameTime);
		mScene->Update(*mGrid, *mCamera, *sViewport, currInput);

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
		mMarkerBlue->Draw(*mCamera, D3DXVECTOR3(5.0f, 1.0f, 5.0f));
		mMarkerBlue->Draw(*mCamera, D3DXVECTOR3(-5.0f, 1.0f, -5.0f));
		mMarkerGreen->Draw(*mCamera, D3DXVECTOR3(-5.0f, 1.0f, 5.0f));

		if(mShowMenu)
			mDragonAgeMenu->Draw();
	}
}
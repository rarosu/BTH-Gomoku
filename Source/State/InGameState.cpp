#include "InGameState.hpp"
#include "Globals.hpp"

namespace State
{
	InGameState::InGameState(StateID id, ID3D10Device* device, const Frustrum& viewFrustrum) :
		ApplicationState(id),
		mDevice(device),
		mGrid(NULL),
		mScene(NULL),
		mCamera(NULL),
		mMarker(NULL),
		mViewFrustrum(viewFrustrum)
	{
		
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
		mScene = new Scene(mDevice);
		mCamera = new Camera(D3DXVECTOR3(0, 0, 0), 
							 D3DXVECTOR3(0, -1.0f, 2.0f), 
							 D3DXVECTOR3(0, 1.0f, 0), 
							 mViewFrustrum);
		mMarker = new Marker(mDevice, 32, D3DXVECTOR3(0, 1.0f, 0));
	}

	void InGameState::OnStatePopped()
	{
		SafeDelete(mGrid);
		SafeDelete(mScene);
		SafeDelete(mCamera);
		SafeDelete(mMarker);
	}

	void InGameState::Update(const InputManager& input, const GameTime& gameTime)
	{
		if (input.GetCurrent().Keyboard.keyIsPressed[VK_ESCAPE])
			QuitApplication();

		mCamera->Update(input.GetPrevious(), input.GetCurrent(), gameTime);
		mScene->Update(mGrid, *mCamera);
		mMarker->Update(*mCamera);
	}

	void InGameState::Draw()
	{
		mScene->Draw();
		mMarker->Draw();
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
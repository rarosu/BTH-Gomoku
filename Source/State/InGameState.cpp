#include "InGameState.hpp"
#include "Globals.hpp"

namespace State
{
	InGameState::InGameState(StateID id, ID3D10Device* device) :
		ApplicationState(id),
		mDevice(device),
		mGrid(NULL),
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

	void InGameState::OnStatePushed()
	{
		mGrid = new Logic::Grid();
		mScene = new Scene(mDevice, CalculateAspectRatio(*sViewport));
	}

	void InGameState::OnStatePopped()
	{
		SafeDelete(mGrid);
		SafeDelete(mScene);
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
	}

	void InGameState::Draw()
	{
		mScene->Draw();
	}
}
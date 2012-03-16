#include "InGameState.hpp"
#include "Globals.hpp"

namespace State
{
	InGameState::InGameState(StateID id, ID3D10Device* device) :
		ApplicationState(id),
		mDevice(device),
		mComponents(NULL),
		mScene(NULL)
	{

	}

	InGameState::~InGameState() throw()
	{

	}

	float CalculateAspectRatio(const Viewport& viewport)
	{
		return static_cast<float>(viewport.GetWidth()) / static_cast<float>(viewport.GetHeight());
	}

	void InGameState::CreateComponents()
	{
		RECT compPos = { 0, 0, 0, 0 };
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "Ingame group", compPos);

		mScene = new Scene(mDevice, mComponents, CalculateAspectRatio(*sViewport));
		mComponents->SetFocusedComponent(mScene);
	}

	void InGameState::OnStatePushed()
	{
		CreateComponents();
		mComponents->SetFocus();
	}

	void InGameState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
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

		//mScene->Update(*mGrid, *sViewport, currInput, prevInput, gameTime);
	}

	void InGameState::Draw()
	{
		mScene->Draw();
	}
}
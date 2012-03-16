#include "AbstractGameState.hpp"

namespace State
{
	AbstractGameState::AbstractGameState(StateID id, ID3D10Device* device)
		: ApplicationState(id)
		, mDevice(device)
		, mScene(NULL)
		, mSession(NULL)
	{

	}

	AbstractGameState::~AbstractGameState() throw()
	{
		SafeDelete(mScene);
		SafeDelete(mSession);
	}


	void AbstractGameState::OnStatePushed()
	{
		CreateComponents();
	}

	void AbstractGameState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;

		SafeDelete(mScene);
		SafeDelete(mSession);
	}


	void AbstractGameState::OnResize()
	{
		mScene->ResizeFrustum(GetAspectRatio());
	}


	void AbstractGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		mSession->Update(gameTime);

		if (currInput.Keyboard.keyIsPressed[VK_RETURN] && !prevInput.Keyboard.keyIsPressed[VK_RETURN])
		{
			if (!mChat->IsVisible())
			{
				mChat->SetVisible(true);
			}
			else
			{
				if (mChat->GetInputFieldContent() == "")
				{
					mChat->SetVisible(false);
				}
			}
		}

		mScene->Update(mSession->GetGrid(), *sViewport, currInput, prevInput, gameTime);
	}

	void AbstractGameState::Draw()
	{
		mScene->Draw();
	}

	void AbstractGameState::SetSession(Logic::Session* session)
	{
		mSession = session;
	}

	void AbstractGameState::CreateComponents()
	{
		const int C_CHAT_HEIGHT = 128;

		RECT r = { 0, 0, 0, 0 };

		mComponents = new Components::ComponentGroup(sRootComponentGroup, "InGameState Group", r);
		mScene = new Scene(mDevice, mComponents, GetAspectRatio());

		r.left = 0;
		r.right = sViewport->GetWidth();
		r.bottom = sViewport->GetHeight();
		r.top = r.bottom - C_CHAT_HEIGHT;
		mChat = new Components::Console(mDevice, mComponents, r, D3DXCOLOR(0.6, 0.6, 0.6, 1.0f));

		mScene->SetFocus();
		mComponents->SetFocus();
	}

	float AbstractGameState::GetAspectRatio()
	{
		return static_cast<float>(sViewport->GetWidth()) / static_cast<float>(sViewport->GetHeight());
	}
}
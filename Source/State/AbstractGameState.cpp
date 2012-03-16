#include "AbstractGameState.hpp"
#include "ComSocket.hpp"

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

		SafeDelete(mSession);
	}


	void AbstractGameState::OnResize()
	{
		mScene->ResizeFrustum(GetAspectRatio());
	}


	void AbstractGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		try
		{
			mSession->Update(gameTime);
		} catch (Network::ConnectionFailure& e)
		{
			MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
			ChangeState(State::C_STATE_MENU);
			return;
		}

		if (currInput.Keyboard.keyIsPressed[VK_TAB] && !prevInput.Keyboard.keyIsPressed[VK_TAB])
		{
			if (!mChat->IsVisible())
			{
				mChat->SetVisible(true);
				mChat->SetFocus();
			}
			else
			{
				mChat->SetVisible(false);
				mScene->SetFocus();
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
		mSession->SetChatReceiver(this);
	}

	void AbstractGameState::ConsoleInputEntered(const Components::Console* consoleInstance, const std::string& message)
	{
		mSession->SendChatMessage(message, -1, Network::Recipient::Broadcast);
	}

	void AbstractGameState::ReceiveChatMessage(const std::string& message, unsigned int sourceID)
	{
		std::string finalMessage = mSession->GetPlayerName(sourceID) + ": " + message; 
		
		mChat->AddLine(finalMessage);
		mChat->SetVisible(true);
		mChat->SetFocus();
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
		mChat->SetInputReceiver(this);
		mChat->SetVisible(false);

		mScene->SetFocus();
		mComponents->SetFocus();
	}

	float AbstractGameState::GetAspectRatio()
	{
		return static_cast<float>(sViewport->GetWidth()) / static_cast<float>(sViewport->GetHeight());
	}
}
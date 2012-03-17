#include "AbstractGameState.hpp"
#include "ComSocket.hpp"

namespace State
{
	AbstractGameState::AbstractGameState(StateID id, ID3D10Device* device)
		: ApplicationState(id)
		, mDevice(device)
		, mScene(NULL)
		, mSession(NULL)
	{}

	AbstractGameState::~AbstractGameState() throw()
	{
		SafeDelete(mSession);
	}


	void AbstractGameState::OnStatePushed()
	{
		CreateComponents();

		InitializeGame();
	}

	void AbstractGameState::OnStatePopped()
	{
		EndGame();		

		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mScene = NULL;
		mChat = NULL;

		SafeDelete(mSession);
	}


	void AbstractGameState::OnResize()
	{
		mScene->ResizeFrustum(GetAspectRatio());
	}


	void AbstractGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		// Update the session
		try
		{
			mSession->Update(gameTime);
		} 
		catch (Network::ConnectionFailure& e)
		{
			MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
			ChangeState(State::C_STATE_MENU);
			return;
		}

		// Toggle chat with TAB
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

		// Update the scene
		mScene->Update(mSession->GetGrid(), currInput, prevInput, gameTime);

		if (mSession->IsLocalPlayerTurn())
		{
			if (currInput.Mouse.buttonIsPressed[C_MOUSE_LEFT] && !prevInput.Mouse.buttonIsPressed[C_MOUSE_LEFT])
			{
				Logic::Cell cell = mScene->PickCell(currInput.Mouse.x, currInput.Mouse.y);
				mSession->SendPlacePieceMessage(cell);
			}
		}
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

	void AbstractGameState::ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message)
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

	void AbstractGameState::SetChatName(const std::string& name)
	{
		mChat->SetName(name);
	}

	void AbstractGameState::CreateComponents()
	{
		const int C_CHAT_HEIGHT = 128;

		RECT r = { 0, 0, 0, 0 };

		mComponents = new Components::ComponentGroup(sRootComponentGroup, "InGameState Group", r);
		mScene = new Scene(mDevice, mComponents, GetAspectRatio(), &mSession->GetGrid(), mSession->GetSlotCount());

		r.left = 0;
		r.right = sViewport->GetWidth();
		r.bottom = sViewport->GetHeight();
		r.top = r.bottom - C_CHAT_HEIGHT;
		mChat = new Components::ChatConsole(mDevice, mComponents, r, D3DXCOLOR(0.6, 0.6, 0.6, 1.0f), "");
		mChat->SetChatReceiver(this);
		mChat->SetVisible(false);

		mScene->SetFocus();
		mComponents->SetFocus();
	}

	float AbstractGameState::GetAspectRatio()
	{
		return static_cast<float>(sViewport->GetWidth()) / static_cast<float>(sViewport->GetHeight());
	}
}
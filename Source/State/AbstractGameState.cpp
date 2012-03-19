#include "AbstractGameState.hpp"
#include "ComSocket.hpp"

namespace State
{
	AbstractGameState::AbstractGameState(StateID id, ID3D10Device* device)
		: ApplicationState(id)
		, mDevice(device)
		, mComponents(NULL)
		, mScene(NULL)
		, mChat(NULL)
		, mSession(NULL)
		, mGameOver(false)
		, mGameOverOverlay(NULL)
	{
		mGameOverOverlay = new Sprite(mDevice, sViewport, "whitePixel.png", sViewport->GetWidth(), sViewport->GetHeight());
	}

	AbstractGameState::~AbstractGameState() throw()
	{
		SafeDelete(mSession);
		SafeDelete(mGameOverOverlay);
	}


	void AbstractGameState::OnStatePushed()
	{
		CreateComponents();

		InitializeGame();

		// Make a quick sanity check - if we don't have a session, the game shouldn't be able to be played.
		if (mSession == NULL)
			mGameOver = true;
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
			// On a disconnect, we simply set the game as game over, but allow the players to remain in game if they wish.
			mGameOver = true;
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

		if (!mGameOver)
		{
			if (mSession->IsLocalPlayerTurn())
			{
				if (currInput.Mouse.buttonIsPressed[C_MOUSE_LEFT] && !prevInput.Mouse.buttonIsPressed[C_MOUSE_LEFT] && mScene->HasFocus())
				{
					Logic::Cell cell = mScene->PickCell(currInput.Mouse.x, currInput.Mouse.y);
					mSession->SendPlacePieceMessage(cell);
				}
			}
		}
	}

	void AbstractGameState::Draw()
	{
		if (mGameOver)
		{
			mGameOverOverlay->Draw(D3DXVECTOR2(0.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.2f));
		}
	}

	void AbstractGameState::ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message)
	{
		if (CanSendChatMessage())
			mSession->SendChatMessage(message, -1, Network::Recipient::Broadcast);
	}	

	void AbstractGameState::ReceiveChatMessage(const std::string& message, Logic::PlayerID sourceID)
	{
		std::string finalMessage = mSession->GetPlayerName(sourceID) + ": " + message; 
		
		mChat->AddLine(finalMessage);
		mChat->SetVisible(true);
		mChat->SetFocus();
	}

	void AbstractGameState::GameOver(Logic::PlayerID winningPlayer)
	{
		// Set the game over flag and notify the players in the chat
		mGameOver = true;

		std::string winner = "! Game won: " + mSession->GetPlayerName(mSession->GetWinner());
		mChat->AddLine(winner);
		mChat->SetFocus();
		mChat->SetVisible(true);
	}

	void AbstractGameState::PlayerConnected(Logic::PlayerID id)
	{
		mChat->AddLine("! Player connected: " + mSession->GetPlayerName(id));
		mChat->SetFocus();
		mChat->SetVisible(true);
	}

	void AbstractGameState::PlayerDisconnected(Logic::PlayerID id, const std::string& name, Network::RemovePlayerReason::RemovePlayerReason reason)
	{
		mChat->AddLine("! Player disconnected: " + name);
		mChat->SetFocus();
		mChat->SetVisible(true);
	}



	void AbstractGameState::SetSession(Logic::Session* session)
	{
		mSession = session;
		mSession->SetSessionNotifiee(this);
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
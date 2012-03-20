#include "AbstractGameState.hpp"
#include "ComSocket.hpp"

namespace State
{
	AbstractGameState::AbstractGameState(StateID id, ID3D10Device* device)
		: ApplicationState(id),
		  mDevice(device),
		  mComponents(NULL),
		  mScene(NULL),
		  mChat(NULL),
		  mGameMenu(NULL),
		  mPlayerList(NULL),
		  mSession(NULL),
		  mGameStage(GameStage::During),
		  mGameOverLabel(NULL),
		  mPieMenu(NULL),
		  mSndPlacePiece(NULL)
	{}

	AbstractGameState::~AbstractGameState() throw()
	{
		SafeDelete(mSession);
	}

	void AbstractGameState::OnStatePushed()
	{
		
		CreateComponents();

		InitializeGame();

		mSndPlacePiece = SoundManager::GetInstance().CreateSound("Resources/Sounds/placeMarker.wav");

		// Make a quick sanity check - if we don't have a session, the game shouldn't be able to be played.
		assert(mSession != NULL);
		mGameStage = GameStage::During;
	}

	void AbstractGameState::OnStatePopped()
	{
		EndGame();		

		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mScene = NULL;
		mChat = NULL;
		mGameOverLabel = NULL;
		mSndPlacePiece = NULL;

		SafeDelete(mSession);
	}

	void AbstractGameState::OnResize()
	{
		mScene->ResizeFrustum(GetAspectRatio());
	}

	void AbstractGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		// Check menu
		if (mGameMenu->GetSubMenu("Menu")->GetAndResetClickStatus("Exit"))
		{
			ChangeState(C_STATE_MENU);

			return;
		}

		// Update the session
		try
		{
			mSession->Update(gameTime);
		} 
		catch (Network::ConnectionFailure& e)
		{
			// On a disconnect, we simply set the game as game over, but allow the players to remain in game if they wish.
			if (mGameStage == GameStage::During)
				OnConnectionFailure();
			mGameStage = GameStage::Aborted;
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

		for (unsigned int i = 0; i < mSession->GetPlayerCount(); ++i)
		{
			if (mPlayerList->GetAndResetClickStatus(i) && mSession->GetSlotType(i) == Logic::Session::C_STATUS_REMOTE)
			{
				mChat->SetInputFieldContent("/" + mSession->GetPlayerName(i) + " ");
				mChat->SetVisible(true);
				mChat->SetFocus();
			}
		}

		// Update the scene
		mScene->Update(mSession->GetGrid(), currInput, prevInput, gameTime);
		mPieMenu->Update(gameTime, currInput, prevInput);



		// Update scene
		Scene::HighlightType highlightType = Scene::None;
		int highlightOption = mPieMenu->GetAndResetClickedItemIndex();
		switch (highlightOption)
		{
			case Components::PieMenu::C_ITEM_NEXT:
				highlightType = Scene::Mine;
			break;

			case Components::PieMenu::C_ITEM_WARNING:
				highlightType = Scene::Warning;
			break;

			case Components::PieMenu::C_ITEM_HINT:
				highlightType = Scene::Hint;
			break;

			case Components::PieMenu::C_ITEM_OTHER:
				highlightType = Scene::General;
			break;
		}

		if (highlightType != Scene::None)
		{
			Logic::Cell pickedCell = mScene->PickCell(mPieMenu->GetPosition().x, mPieMenu->GetPosition().y);
			
			if (mScene->GetCellHighlight(pickedCell) == highlightType)
				highlightType = Scene::None;

			mScene->HighlightCell(pickedCell, highlightType);
			mSession->SendHighlightMessage(pickedCell, GetHighlightConstant(highlightType));
		}
		
		
		Logic::Cell pickedCell = mScene->PickCell(currInput.Mouse.x, currInput.Mouse.y);
		if (mGameStage == GameStage::During)
		{
			if (mSession->IsLocalPlayerTurn())
			{
				if (currInput.Mouse.buttonIsPressed[C_MOUSE_LEFT] && !prevInput.Mouse.buttonIsPressed[C_MOUSE_LEFT] && mScene->HasFocus()
					&& !(mGameMenu->IsHovered() || mPlayerList->IsHovered()))
				{
					mSession->SendPlacePieceMessage(pickedCell);
				}
			}
		}
	}

	void AbstractGameState::Draw()
	{}

	void AbstractGameState::ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message, Logic::PlayerID target, Network::Recipient::Recipient recipient)
	{
		if (CanSendChatMessage())
			mSession->SendChatMessage(message, target, recipient);
	}	

	void AbstractGameState::ReceiveChatMessage(const std::string& message, Network::Recipient::Recipient recipient, Logic::PlayerID sourceID)
	{
		if (mChat != NULL)
		{
			mChat->AddIncomingMessage(message, recipient, sourceID);
			mChat->SetFocus();
			mChat->SetVisible(true);
		}
	}

	void AbstractGameState::PlacePiece(Logic::PlayerID id, const Logic::Cell& cell)
	{
		mScene->LookAtCell(cell);
		SoundManager::GetInstance().PlaySound(mSndPlacePiece);
	}

	void AbstractGameState::SetTeam(Logic::PlayerID id, Logic::Team team)
	{
		assert(false);
	}

	void AbstractGameState::GameOver(Logic::PlayerID winningPlayer)
	{
		// Set the game over flag and notify the players in the chat
		mGameStage = GameStage::Won;

		std::string winners;
		if (mSession->GetSlotCount() == 2)
		{
			winners = mSession->GetPlayerName(mSession->GetWinner());
		}
		else if (mSession->GetSlotCount() == 4)
		{
			winners = mSession->GetPlayerName(mSession->GetWinner()) + " & " + mSession->GetPlayerName(mSession->GetTeamMate(mSession->GetWinner()));
		}

		std::string winner = "! Game won: " + winners;
		mChat->AddLine(winner);
		mChat->SetFocus();
		mChat->SetVisible(true);
		mGameOverLabel->SetVisible(true);
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

	void AbstractGameState::SetHighlightedCell(const Logic::Cell& cell, int highlightType)
	{
		mScene->HighlightCell(cell, GetHighlightType(highlightType));
		mScene->LookAtCell(cell);

		mChat->AddLine("! Highlighted cell: " + GetHighlightName(GetHighlightType(highlightType)));
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
		const int C_MENU_WIDTH = 128;
		const int C_MENU_HEIGHT = 48;
		const int C_PLIST_WIDTH = 200;
		const int C_PLIST_HEIGHT = 48;

		RECT r = { 0, 0, 0, 0 };

		mComponents = new Components::ComponentGroup(sRootComponentGroup, "InGameState Group", r);
		mScene = new Scene(mDevice, mComponents, GetAspectRatio(), &mSession->GetGrid(), mSession->GetSlotCount());

		r.left = 0;
		r.right = 0;
		r.top = 0;
		r.bottom = 0;
		mGameMenu = new Components::ClickMenu(mComponents, mDevice, r, C_MENU_WIDTH, C_MENU_HEIGHT);
		mGameMenu->AddMenuItem("Menu");
		mGameMenu->GetMenuItem("Menu")->AddSubItem("Exit");

		r.left = sViewport->GetWidth() - C_PLIST_WIDTH;
		r.top = 0;
		mPlayerList = new Components::PlayerList(mSession, mDevice, mComponents, r, C_PLIST_WIDTH, C_PLIST_HEIGHT);

		r.left = 0;
		r.right = sViewport->GetWidth();
		r.top = 0;
		r.bottom = 100;
		mGameOverLabel = new Components::Label(mDevice, mComponents, "GAME OVER", r);
		mGameOverLabel->SetVisible(false);

		r.left = 0;
		r.top = 0;
		r.right = 0;
		r.bottom = 0;
		mPieMenu = new Components::PieMenu(mComponents, mDevice, r);
		mPieMenu->Initialize();

		r.left = 0;
		r.right = sViewport->GetWidth();
		r.bottom = sViewport->GetHeight();
		r.top = r.bottom - C_CHAT_HEIGHT;
		mChat = new Components::ChatConsole(mSession, mDevice, mComponents, r, D3DXCOLOR(0.6, 0.6, 0.6, 1.0f), "");
		mChat->SetChatReceiver(this);
		mChat->SetVisible(false);

		mScene->SetFocus();
		mComponents->SetFocus();
	}

	float AbstractGameState::GetAspectRatio()
	{
		return static_cast<float>(sViewport->GetWidth()) / static_cast<float>(sViewport->GetHeight());
	}

	int AbstractGameState::GetHighlightConstant(Scene::HighlightType type) const
	{
		int constant = 0;
		switch (type)
		{
			case Scene::None:
				constant = 0;
			break;

			case Scene::Hint:
				constant = 1;
			break;

			case Scene::Warning:
				constant = 2;
			break;

			case Scene::General:
				constant = 3;
			break;

			case Scene::Mine:
				constant = 4;
			break;
		}

		return constant;
	}

	Scene::HighlightType AbstractGameState::GetHighlightType(int highlightConstant) const
	{
		Scene::HighlightType type = Scene::None;
		switch (highlightConstant)
		{
			case 0:
				type = Scene::None;
			break;

			case 1:
				type = Scene::Hint;
			break;

			case 2:
				type = Scene::Warning;
			break;

			case 3:
				type = Scene::General;
			break;

			case 4:
				type = Scene::Mine;
			break;
		}

		return type;
	}

	std::string AbstractGameState::GetHighlightName(Scene::HighlightType type) const
	{
		std::string name;
		switch (type)
		{
			case Scene::None:
				name = "None";
			break;

			case Scene::Hint:
				name = "Hint";
			break;

			case Scene::Warning:
				name = "Warning";
			break;

			case Scene::General:
				name = "General";
			break;

			case Scene::Mine:
				name = "Mine";
			break;
		}

		return name;
	}
}
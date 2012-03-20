#include "CreateGameState.hpp"
#include "ServerSession.hpp"
#include "StandardRuleset.hpp"
#include "Ruleset2v2.hpp"

namespace State
{
	CreateGameState::CreateGameState(StateID id, ID3D10Device* device, ServerLobbyState* serverLobbyState)
		: ApplicationState(id),
		  mDevice(device), 
		  mComponents(NULL), 
		  mDefaultFont(NULL),
		  mServerLobbyState(serverLobbyState),
		  mIFName(NULL),
		  mIFPort(NULL),
		  mBtnCreate(NULL),
		  mBtnCancel(NULL),
		  mGameType(NULL),
		  mPlayerType(NULL),
		  mBackground(NULL),
		  mJustChosenG(false),	// DEBUG
		  mJustChosenP(false)	// DEBUG
	{
		mDefaultFont = new GameFont(mDevice, "Segoe Print", 48);
		mBackground = new Sprite(mDevice, sViewport, "marbleBG1422x800.png", sViewport->GetWidth(), sViewport->GetHeight());
	}

	CreateGameState::~CreateGameState() throw()
	{
		SafeDelete(mDefaultFont);
		SafeDelete(mBackground);
	}

	void CreateGameState::CreateComponents()
	{
		// Define sizes
		const int C_BUTTON_WIDTH = 192;
		const int C_BUTTON_HEIGHT = mDefaultFont->GetSize();
		const int C_INPUT_FIELD_WIDTH = 300;
		const int C_INPUT_FIELD_HEIGHT = mDefaultFont->GetSize();
		const int C_LABEL_WIDTH = 180;
		const int C_LABEL_HEIGHT = mDefaultFont->GetSize();
		const int C_OFFSET_LEFT = 180;
		const int C_OFFSET_TOP = 180 - C_LABEL_HEIGHT * 1.5; // DEBUG: When Game Type is invisible
		RECT r;

		// Create all the components and put them in the right place
		RECT compPos = { 0, 0, 0, 0 };
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "CreateGameState Group", compPos);

		r.left = 100;
		r.right = sViewport->GetWidth() - r.left;
		r.top = 30;
		r.bottom = 150;
		new Components::Label(mDevice, mComponents, "CREATE A GAME", r);

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 3;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "Name:", r, 0, GameFont::Right);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH + 20;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mIFName = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont, 14);
		mIFName->SetText("Admin");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 4.5;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "Port:", r, 0, GameFont::Right);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH + 20;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mIFPort = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont, 14);
		mIFPort->SetText("6666");

		r.left = r.right - C_BUTTON_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 6;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mBtnCancel = new Components::TextButton(mComponents, r);
		mBtnCancel->Initialize(mDevice, "Cancel");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_BUTTON_WIDTH;
		mBtnCreate = new Components::TextButton(mComponents, r);
		mBtnCreate->Initialize(mDevice, "Create Game");

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH + 20;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.top = C_OFFSET_TOP;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mGameType = new Components::ClickMenu(mComponents, mDevice, r, r.right - r.left, r.bottom - r.top);
		mGameType->AddMenuItem("Choose Game Type");
		mGameType->GetMenuItem(0)->AddSubItem("Normal Game");
		mGameType->GetMenuItem(0)->AddSubItem("Crazy Game");
		mGameType->SetVisible(false);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH + 20;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 1.5;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mPlayerType = new Components::ClickMenu(mComponents, mDevice, r, r.right - r.left, r.bottom - r.top);
		mPlayerType->AddMenuItem("Choose Players");
		mPlayerType->GetMenuItem(0)->AddSubItem("1 v 1 Player");
		mPlayerType->GetMenuItem(0)->AddSubItem("2 v 2 Player");

		// Set the initial focus, and focus this component group
		mIFName->SetFocus();
		mComponents->SetFocus();

		// Reset chosen game type
		mChosenGame = GameType();
	}

	void CreateGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{	
		if (mBtnCancel->GetAndResetClickStatus())
		{
			ChangeState(C_STATE_MENU);
			return;
		}

		// Check if a player type has been chosen
		if (mPlayerType->GetSubMenu(0)->GetAndResetClickStatus(0))
		{
			if(!mJustChosenP) // DEBUG
			{
				mPlayerType->GetMenuItem(0)->SetCaption(mPlayerType->GetSubMenu(0)->GetMenuItem(0)->GetCaption());
				mPlayerType->GetSubMenu(0)->SetVisible(false);
				mChosenGame.mPlayers = GameType::Players1v1;
				mJustChosenP = true; // DEBUG
			}
			else
				mJustChosenP = false; // DEBUG
		}
		else if (mPlayerType->GetSubMenu(0)->GetAndResetClickStatus(1))
		{
			if(!mJustChosenP) // DEBUG
			{
				mPlayerType->GetMenuItem(0)->SetCaption(mPlayerType->GetSubMenu(0)->GetMenuItem(1)->GetCaption());
				mPlayerType->GetSubMenu(0)->SetVisible(false);
				mChosenGame.mPlayers = GameType::Players2v2;
				mJustChosenP = true; // DEBUG
			}
			else
				mJustChosenP = false; // DEBUG
		}

		// Check if a game type has been chosen
		if (mGameType->GetSubMenu(0)->GetAndResetClickStatus(0))
		{
			if(!mJustChosenG) // DEBUG
			{
				mGameType->GetMenuItem(0)->SetCaption(mGameType->GetSubMenu(0)->GetMenuItem(0)->GetCaption());
				mGameType->GetSubMenu(0)->SetVisible(false);
				mChosenGame.mType = GameType::Normal;
				mJustChosenG = true; // DEBUG
			}
			else
				mJustChosenG = false; // DEBUG
		}
		else if (mGameType->GetSubMenu(0)->GetAndResetClickStatus(1))
		{
			if(!mJustChosenG) // DEBUG
			{
				mGameType->GetMenuItem(0)->SetCaption(mGameType->GetSubMenu(0)->GetMenuItem(1)->GetCaption());
				mGameType->GetSubMenu(0)->SetVisible(false);
				mChosenGame.mType = GameType::Crazy;
				mJustChosenG = true; // DEBUG
			}
			else
				mJustChosenG = false; // DEBUG
		}



		// Basic check to see if the port is valid
		unsigned short port = 0;
		std::stringstream s;
		s.str(mIFPort->GetText());
		if (!(s >> port))
		{
			mBtnCreate->SetEnabled(false);
			return;
		}

		// Basic check to see if the name is valid (non-empty)
		if (mIFName->IsEmpty())
		{
			mBtnCreate->SetEnabled(false);
			return;
		}

		// All is fine (so far)! Allow creation of server.
		mBtnCreate->SetEnabled(true);

		if (mBtnCreate->GetAndResetClickStatus())
		{
			Logic::Ruleset* ruleset;
			switch (mChosenGame.mPlayers)
			{
				case GameType::Players1v1:
					ruleset = new Logic::StandardRuleset();
				break;

				case GameType::Players2v2:
					ruleset = new Logic::Ruleset2v2();
				break;
			}
			
			try
			{
				Network::Server* server = new Network::Server(ruleset->GetPlayerCount(), port);
				mServerLobbyState->SetSessionArguments(server, mIFName->GetText(), ruleset);
				
				ChangeState(C_STATE_SERVER_LOBBY);
			}
			catch (Network::BindException& e)
			{
				// TODO: Report error here in some other way
				std::stringstream ss;
				ss << "Failed to bind to port: " << e.GetPort() << " with error: " << e.GetErrorCode();

				MessageBox(NULL, ss.str().c_str(), "Bind Error", MB_OK | MB_ICONERROR);
			}
		}
	}

	void CreateGameState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0, 0));
	}

	void CreateGameState::OnStatePushed()
	{
		CreateComponents();
	}

	void CreateGameState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);

		mComponents = NULL;
		mIFName = NULL;
		mIFPort = NULL;
		mBtnCreate = NULL;
		mBtnCancel = NULL;
		mPlayerType = NULL;
		mGameType = NULL;
	}
}
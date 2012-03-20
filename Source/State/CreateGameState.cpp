#include "CreateGameState.hpp"
#include "ServerSession.hpp"
#include "StandardRuleset.hpp"
#include "Ruleset2v2.hpp"
#include <fstream>

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
		  mBackground(NULL)
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
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 3.0;
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

		LoadSetupInfo();

		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 6.0;
		r.bottom = r.top + C_LABEL_HEIGHT;
		mBtnCreate = new Components::TextButton(mComponents, r);
		mBtnCreate->Initialize(mDevice, "Create Game");

		
		/*r.right = sViewport->GetWidth() - 20;
		r.left = r.right - mBtnCreate->GetWidth();*/
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 7.5;
		r.bottom = r.top + C_LABEL_HEIGHT;
		mBtnCancel = new Components::TextButton(mComponents, r);
		mBtnCancel->Initialize(mDevice, "Cancel");

		
		std::vector<std::string> gameItems;
		gameItems.push_back("Normal Game");
		gameItems.push_back("Crazy Game");
		
		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH + 20;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.top = C_OFFSET_TOP;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mGameType = new Components::ToggleButton(mComponents, r, gameItems);
		mGameType->Initialize(mDevice);
		mGameType->SetVisible(false);
		
		
		std::vector<std::string> playerItems;
		playerItems.push_back("1 v 1 Player");
		playerItems.push_back("2 v 2 Player");

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH + 20;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 1.5;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mPlayerType = new Components::ToggleButton(mComponents, r, playerItems);
		mPlayerType->Initialize(mDevice);

		// Set the initial focus, and focus this component group
		mIFName->SetFocus();
		mComponents->SetFocus();
	}

	void CreateGameState::SaveSetupInfo()
	{
		std::ofstream file;

		file.open("Resources/GameData/CreateGameInfo.txt", std::ios::out);
		if(file.is_open())
		{
			file << mIFName->GetText() << "\n";
			file << mIFPort->GetText() << "\n";
		}
	}

	void CreateGameState::LoadSetupInfo()
	{
		std::ifstream file;

		file.open("Resources/GameData/CreateGameInfo.txt", std::ios::in);
		if(file.is_open())
		{
			std::string name, port;
			std::getline(file, name);
			std::getline(file, port);

			if(mIFName != NULL)
				mIFName->SetText(name);

			if(mIFPort != NULL)
				mIFPort->SetText(port);
		}
	}

	void CreateGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{	
		if (mBtnCancel->GetAndResetClickStatus())
		{
			ChangeState(C_STATE_MENU);
			return;
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
			switch (mPlayerType->GetCurrentValue())
			{
				case 0:
					ruleset = new Logic::StandardRuleset();
				break;

				case 1:
					ruleset = new Logic::Ruleset2v2();
				break;
			}
			
			try
			{
				std::string newName("");
				std::string oldName = mIFName->GetText();

				for(int i = 0; i < oldName.length(); ++i)
				{
					if(oldName[i] != ' ')
						newName += oldName[i];
				}
				mIFName->SetText(newName);

				Network::Server* server = new Network::Server(ruleset->GetPlayerCount(), port);
				mServerLobbyState->SetSessionArguments(server, mIFName->GetText(), ruleset);

				SaveSetupInfo();
				
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
		mGameType = NULL;
		mPlayerType = NULL;
	}
}
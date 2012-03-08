#include "CreateGameState.hpp"
#include "ServerSession.hpp"

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
		  mBtnCancel(NULL)
	{

		mDefaultFont = new GameFont(mDevice, "Segoe Print", 48);
	}

	CreateGameState::~CreateGameState() throw()
	{
		SafeDelete(mDefaultFont);
		SafeDelete(mComponents);
	}

	void CreateGameState::CreateComponents()
	{
		// Define sizes
		const int C_OFFSET_LEFT = 100;
		const int C_OFFSET_TOP = 430;
		const int C_BUTTON_WIDTH = 192;
		const int C_BUTTON_HEIGHT = mDefaultFont->GetSize();
		const int C_INPUT_FIELD_WIDTH = 192;
		const int C_INPUT_FIELD_HEIGHT = mDefaultFont->GetSize();
		const int C_LABEL_WIDTH = 220;
		const int C_LABEL_HEIGHT = mDefaultFont->GetSize();
		RECT r;

		// Create all the components and put them in the right place
		RECT compPos = { 0, 0, 0, 0 };
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "CreateGameState Group", compPos);

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "Name:", r);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mIFName = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mIFName->SetText("Admin");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 2;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "Port:", r);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mIFPort = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mIFPort->SetText("6666");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_BUTTON_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 4;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mBtnCreate = new Components::TextButton(mComponents, r);
		mBtnCreate->Initialize(mDevice, "Create Game");

		r.left = C_OFFSET_LEFT + C_BUTTON_WIDTH * 2;
		r.right = r.left + C_BUTTON_WIDTH;
		mBtnCancel = new Components::TextButton(mComponents, r);
		mBtnCancel->Initialize(mDevice, "Cancel");



		// Set the initial focus, and focus this component group
		mIFName->SetFocus();
		mComponents->SetFocus();	
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
		if (mIFName->Empty())
		{
			mBtnCreate->SetEnabled(false);
			return;
		}

		// All is fine (so far)! Allow creation of server.
		mBtnCreate->SetEnabled(true);

		if (mBtnCreate->GetAndResetClickStatus())
		{
			Logic::Ruleset* ruleset = new Logic::StandardRuleset();
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
	}
}
#include "JoinGameState.hpp"
#include "Label.hpp"
#include "ClientSession.hpp"
#include "MessageInclude.hpp"
#include <sstream>

namespace State
{
	JoinGameState::JoinGameState(StateID id, ID3D10Device* device, ClientLobbyState* lobbyState)
		: ApplicationState(id), 
		  mDevice(device),
		  mClientLobbyState(lobbyState),
		  mDefaultFont(NULL),
		  mBackground(NULL),
		  mComponents(NULL),
		  mNameField(NULL),
		  mIPAddressField(NULL),
		  mPortField(NULL),
		  mJoinButton(NULL),
		  mCancelButton(NULL),
		  mClient(NULL)
	{
		mDefaultFont = new GameFont(mDevice,  "Segoe Print", 48);
		mBackground = new Sprite(mDevice, sViewport, "marbleBG1422x800.png", sViewport->GetWidth(), sViewport->GetHeight());
	}

	JoinGameState::~JoinGameState() throw()
	{
		SafeDelete(mDefaultFont);
		SafeDelete(mBackground);
		SafeDelete(mClient);
	}

	void JoinGameState::OnStatePushed()
	{
		CreateComponents();
	}

	void JoinGameState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);

		mComponents = NULL;
		mNameField = NULL;
		mIPAddressField = NULL;
		mPortField = NULL;
		mJoinButton = NULL;
		mCancelButton = NULL;
	}

	void JoinGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		// Check cancel button
		if (mCancelButton->GetAndResetClickStatus())
		{
			SafeDelete(mClient);

			ChangeState(C_STATE_MENU);
			return;
		}

		if (mClient != NULL)
		{
			mClient->Update();

			for (int i = mClient->GetQueuedMessageCount(); i >= 0; --i)
			{
				switch (mClient->PeekMessageID(i))
				{
					case Network::C_MESSAGE_ACCEPT:
					{
						Network::AcceptMessage* m = static_cast<Network::AcceptMessage*>(mClient->PopMessage(i));

						mClientLobbyState->SetSessionArguments(mClient, m->mNumberOfPlayers, m->mSelfID, mNameField->GetText());
						ChangeState(C_STATE_CLIENT_LOBBY);

						SafeDelete(m);
					} break;

					case Network::C_MESSAGE_REFUSE:
					{
						Network::RefuseMessage* m = static_cast<Network::RefuseMessage*>(mClient->PopMessage(i));

						switch (m->mReason)
						{
							case Network::RefuseReason::TooManyPlayers:
								MessageBox(NULL, "Connection refused: Too many players", "Error", MB_OK | MB_ICONERROR);
							break;

							case Network::RefuseReason::InvalidName:
								MessageBox(NULL, "Connection refused: Invalid name, please change it", "Error", MB_OK | MB_ICONERROR);
							break;
						}

						mNameField->SetEnabled(true);
						mIPAddressField->SetEnabled(true);
						mPortField->SetEnabled(true);
						mJoinButton->SetEnabled(true);

						SafeDelete(m);
					} break;
				}
			}
		}

		// Basic check to see if the name is valid (non-empty)
		if (mNameField->IsEmpty())
		{
			mJoinButton->SetEnabled(false);
			return;
		}

		// Basic check to see if the IP/Hostname is valid (non-empty)
		if (mIPAddressField->IsEmpty())
		{
			mJoinButton->SetEnabled(false);
			return;
		}

		// Basic check to see if the port is valid
		std::stringstream s(mPortField->GetText());
		unsigned short port = 0;
		if (!(s >> port))
		{
			mJoinButton->SetEnabled(false);
			return;
		}

		// If we're currently attempting a connect, do not allow joining again
		if (mClient != NULL)
		{
			mJoinButton->SetEnabled(false);
			return;
		}

		// All is fine (so far)! Allow creation of client.
		mJoinButton->SetEnabled(true);
		
		if (mJoinButton->GetAndResetClickStatus())
		{
			try
			{
				mClient = new Network::Client(mIPAddressField->GetText().c_str(), port);
				mClient->Send(Network::JoinMessage(mNameField->GetText()));

				mNameField->SetEnabled(false);
				mIPAddressField->SetEnabled(false);
				mPortField->SetEnabled(false);
			}
			catch (Network::ConnectionFailure& e)
			{
				MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
			}
			
			
		}
	}

	void JoinGameState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0, 0));
	}

	void JoinGameState::CreateComponents()
	{
		// Define sizes
		const int C_OFFSET_LEFT = 100;
		const int C_OFFSET_TOP = 100;
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
		mNameField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mNameField->SetText("Player");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 2;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "IP/Hostname:", r);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mIPAddressField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mIPAddressField->SetText("127.0.0.1");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 4;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "Port:", r);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mPortField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mPortField->SetText("6666");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_BUTTON_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 6;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mJoinButton = new Components::TextButton(mComponents, r);
		mJoinButton->Initialize(mDevice, "Join Game");

		r.left = C_OFFSET_LEFT + C_BUTTON_WIDTH * 2;
		r.right = r.left + C_BUTTON_WIDTH;
		mCancelButton = new Components::TextButton(mComponents, r);
		mCancelButton->Initialize(mDevice, "Cancel");



		// Set the initial focus, and focus this component group
		mNameField->SetFocus();
		mComponents->SetFocus();
	}

	
}
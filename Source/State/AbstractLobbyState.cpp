#include "AbstractLobbyState.hpp"
#include "ComSocket.hpp"
#include <cassert>
#include <sstream>

namespace State
{
	AbstractLobbyState::AbstractLobbyState(StateID id, ID3D10Device* device)
		: ApplicationState(id),
		mDevice(device),
		mBackground(NULL),
		mComponents(NULL),
		mCancelButton(NULL),
		mChat(NULL),
		mSession(NULL)
	{
		mBackground = new Sprite(mDevice, sViewport, "marbleBG1422x800.png", sViewport->GetWidth(), sViewport->GetHeight());
	}

	AbstractLobbyState::~AbstractLobbyState() throw()
	{
		SafeDelete(mBackground);
		SafeDelete(mSession);
	}

	void AbstractLobbyState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		// Check if we want to cancel
		if(mCancelButton->GetAndResetClickStatus())
		{
			SafeDelete(mSession);
			ChangeState(C_STATE_MENU);
			
			return;
		}

		// Update the session and check for errors
		try
		{
			mSession->Update(gameTime);
		} 
		catch (Network::ConnectionFailure& e)
		{
			MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
			
			SafeDelete(mSession);
			ChangeState(C_STATE_MENU);

			return;
		}

		// Update the player labels
		for (unsigned int i = 0; i < mSession->GetSlotCount(); ++i)
		{
			std::stringstream s;
			s << (i + 1) << ". " << mSession->GetPlayerName(i);

			mPlayerLabels[i]->SetCaption(s.str());
		}

		// Let the subclass define its own logic, if it wishes.
		AppendUpdate();
	}

	void AbstractLobbyState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0.0f, 0.0f));
	}

	void AbstractLobbyState::OnStatePushed()
	{
		// We shouldn't enter this state until the session has been set
		assert(mSession != NULL);

		CreateComponents();
	}

	void AbstractLobbyState::OnStatePopped()
	{
		// Allow the subclass to do cleanup of its own.
		AppendStatePopped();

		mSession = NULL;

		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mCancelButton = NULL;
		mChat = NULL;
		mPlayerLabels.clear();
	}

	void AbstractLobbyState::ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message, Logic::PlayerID target, Network::Recipient::Recipient recipient)
	{
		if (consoleInstance == mChat)
		{
			mSession->SendChatMessage(message, target, recipient);
		}
	}

	void AbstractLobbyState::ReceiveChatMessage(const std::string& message, Network::Recipient::Recipient recipient, Logic::PlayerID sourceID)
	{
		if (mChat != NULL)
			mChat->AddIncomingMessage(message, recipient, sourceID);
	}

	void AbstractLobbyState::PlacePiece(Logic::PlayerID id, const Logic::Cell& cell)
	{
		// TODO: Stuff
	}

	void AbstractLobbyState::GameOver(Logic::PlayerID winningPlayer)
	{
		assert(false && "Game over event cannot happen in the lobby");
	}

	void AbstractLobbyState::PlayerConnected(Logic::PlayerID id)
	{
		mChat->AddLine("! Player connected: " + mSession->GetPlayerName(id));
	}

	void AbstractLobbyState::PlayerDisconnected(Logic::PlayerID id, const std::string& name, Network::RemovePlayerReason::RemovePlayerReason reason)
	{
		mChat->AddLine("! Player disconnected: " + name);
	}
	
	void AbstractLobbyState::SetSession(Logic::Session* session)
	{
		mSession = session;
		mSession->SetSessionNotifiee(this);
	}

	void AbstractLobbyState::CreateComponents()
	{
		const int C_LABEL_WIDTH		= 150;
		const int C_LABEL_HEIGHT	= 50;
		const int C_LABEL_MARGIN	= 0;
		const int C_BUTTON_XOFFSET	= 20;
		const int C_BUTTON_YOFFSET	= 20;
		const int C_BUTTON_WIDTH	= 150;
		const int C_BUTTON_HEIGHT	= 50;
		const int C_CHAT_HEIGHT		= 150;

		// Create the component groups
		RECT r = { 0, 0, 0, 0 };
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "LobbyState Group", r);

		r.left = 100;
		r.right = sViewport->GetWidth() - r.left;
		r.top = 30;
		r.bottom = 150;
		new Components::Label(mDevice, mComponents, "GAME LOBBY", r);

		// Create the labels
		r.left = 50;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = 150;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "PLAYERS:", r);
		r.bottom += 10;

		for (unsigned int i = 0; i < 4/*mSession->GetSlotCount()*/; ++i)
		{
			r.top = r.bottom + C_LABEL_MARGIN;
			r.bottom = r.top + C_LABEL_HEIGHT;

			mPlayerLabels.push_back(new Components::Label(mDevice, mComponents, "1.", r, 0, GameFont::Left));
		}

		// Create the cancel button
		r.left = sViewport->GetWidth() - (C_BUTTON_WIDTH + C_BUTTON_XOFFSET);
		r.right = r.left + C_BUTTON_WIDTH;
		r.bottom = sViewport->GetHeight() - (C_CHAT_HEIGHT + C_BUTTON_YOFFSET);
		r.top = r.bottom - C_BUTTON_HEIGHT;
		mCancelButton = new Components::TextButton(mComponents, r);
		mCancelButton->Initialize(mDevice, "Cancel");

		// Create chat
		r.left = 0;
		r.right = sViewport->GetWidth();
		r.top = sViewport->GetHeight() - C_CHAT_HEIGHT;
		r.bottom = sViewport->GetHeight();
		mChat = new Components::ChatConsole(mSession, mDevice, mComponents, r, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), "");
		mChat->SetChatReceiver(this);

		// Set initial focus
		mChat->SetFocus();
		mComponents->SetFocus();

		// Let the subclass specify more components if it wishes.
		AppendComponents();
	}
}
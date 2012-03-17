#include "AbstractLobbyState.hpp"
#include "ComSocket.hpp"
#include <cassert>
#include <sstream>

namespace State
{
	AbstractLobbyState::AbstractLobbyState(StateID id, ID3D10Device* device)
		: ApplicationState(id)
		, mDevice(device)
		, mBackground(NULL)
		, mComponents(NULL)
		, mCancelButton(NULL)
		, mChat(NULL)
		, mSession(NULL)
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


	void AbstractLobbyState::ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message)
	{
		if (consoleInstance == mChat)
		{
			mSession->SendChatMessage(message, -1, Network::Recipient::Broadcast);
		}
	}

	void AbstractLobbyState::ReceiveChatMessage(const std::string& message, unsigned int sourceID)
	{
		std::string entry = mSession->GetPlayerName(sourceID) + ": " + message;

		if (mChat != NULL)
			mChat->AddLine(entry);
	}

	void AbstractLobbyState::SetSession(Logic::Session* session)
	{
		mSession = session;
		mSession->SetChatReceiver(this);
	}



	void AbstractLobbyState::CreateComponents()
	{
		// Create the component groups
		RECT r = { 0, 0, 0, 0 };
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "LobbyState Group", r);

		// Create the labels
		r.left = 50;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = 50;
		r.bottom = r.top + C_LABEL_HEIGHT;

		for (unsigned int i = 0; i < mSession->GetSlotCount(); ++i)
		{
			mPlayerLabels.push_back(new Components::Label(mDevice, mComponents, "1.", r, GameFont::Left));

			r.top = r.bottom + C_LABEL_MARGIN;
			r.bottom = r.top + C_LABEL_HEIGHT;
		}

		// Create the cancel button
		r.left = 50;
		r.right = r.left + C_BUTTON_WIDTH;
		r.top += C_BUTTON_YOFFSET;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mCancelButton = new Components::TextButton(mComponents, r);
		mCancelButton->Initialize(mDevice, "Cancel");

		// Create chat
		r.left = 0;
		r.right = sViewport->GetWidth();
		r.top = sViewport->GetHeight() - C_CHAT_HEIGHT;
		r.bottom = sViewport->GetHeight();
		mChat = new Components::ChatConsole(mDevice, mComponents, r, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), "");
		mChat->SetChatReceiver(this);

		// Set initial focus
		mChat->SetFocus();
		mComponents->SetFocus();

		// Let the subclass specify more components if it wishes.
		AppendComponents();
	}
}
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
		mSession(NULL),
		mDefaultFont(NULL)
	{
		mBackground = new Sprite(mDevice, sViewport, "marbleBG1422x800.png", sViewport->GetWidth(), sViewport->GetHeight());
		mDefaultFont = new GameFont(mDevice);
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

		// Update the player labels
		for (unsigned int i = 0; i < mSession->GetSlotCount(); ++i)
		{
			std::stringstream s;
			s << (i + 1) << ". " << mSession->GetPlayerName(i);

			mPlayerLabels[i]->SetCaption(s.str());

			/*
			mTeamMenus[i]->SetCurrentValue(mSession->GetPlayerTeam(i));
			switch (mSession->GetSlotType(i))
			{
				case Logic::Session::C_STATUS_LOCAL:
					mSlotMenus[i]->SetCurrentValue(0);
				break;

				case Logic::Session::C_STATUS_OPEN:
				case Logic::Session::C_STATUS_REMOTE:
					mSlotMenus[i]->SetCurrentValue(1);
				break;
			}
			*/
			
		}

		HandleMenus();

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
		AppendStatePushed();
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
		mTeamMenus.clear();
		mSlotMenus.clear();
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
		assert(false);
	}

	void AbstractLobbyState::SetTeam(Logic::PlayerID id, Logic::Team team)
	{
		mTeamMenus[id]->SetCurrentValue(team);
	}

	void AbstractLobbyState::GameOver(Logic::PlayerID winningPlayer)
	{
		assert(false && "Game over event cannot happen in the lobby");
	}

	void AbstractLobbyState::PlayerConnected(Logic::PlayerID id)
	{
		mChat->AddLine("! Player connected: " + mSession->GetPlayerName(id));
		AppendClientConnected(id);
	}

	void AbstractLobbyState::PlayerDisconnected(Logic::PlayerID id, const std::string& name, Network::RemovePlayerReason::RemovePlayerReason reason)
	{
		mChat->AddLine("! Player disconnected: " + name);

		mTeamMenus[id]->SetCurrentValue(0);
		mTeamMenus[id]->SetEnabled(false);
		AppendClientDisconnected(id);
	}
	
	void AbstractLobbyState::SetSession(Logic::Session* session)
	{
		mSession = session;
		mSession->SetSessionNotifiee(this);
	}

	void AbstractLobbyState::CreateComponents()
	{
		const int C_MENU_WIDTH		= 180;
		const int C_MENU_SLOT_WIDTH	= 120;
		const int C_MENU_XOFFSET	= 10;
		const int C_LABEL_WIDTH		= 300;
		const int C_LABEL_HEIGHT	= 50;
		const int C_LABEL_MARGIN	= 10;
		const int C_LABEL_XOFFSET	= C_MENU_XOFFSET + C_MENU_SLOT_WIDTH + C_LABEL_MARGIN;
		const int C_LABEL_YOFFSET	= 150 + C_LABEL_HEIGHT + 10;
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

		// Create chat
		r.left = 0;
		r.right = sViewport->GetWidth();
		r.top = sViewport->GetHeight() - C_CHAT_HEIGHT;
		r.bottom = sViewport->GetHeight();
		mChat = new Components::ChatConsole(mSession, mDevice, mComponents, r, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), "");
		mChat->SetChatReceiver(this);

		// Create title label.
		r.left = C_LABEL_XOFFSET;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = 150;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "PLAYERS:", r);
		r.bottom = C_LABEL_YOFFSET;

		for (unsigned int i = 0; i < mSession->GetSlotCount(); ++i)
		{
			r.left = C_LABEL_XOFFSET;
			r.right = r.left + C_LABEL_WIDTH;
			r.top = r.bottom;
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

		// Let the subclass specify more components if it wishes.
		AppendComponents();

		// Create team and slot menus
		r.left = C_MENU_XOFFSET;
		r.right = r.left + C_MENU_SLOT_WIDTH;
		r.top = C_LABEL_YOFFSET;
		r.bottom = r.top + C_LABEL_HEIGHT;
		
		std::vector<std::string> slotItems;
		slotItems.push_back("Local");
		slotItems.push_back("Network");

		for (unsigned int i = 0; i < mSession->GetSlotCount(); ++i)
		{
			mSlotMenus.push_back(new Components::ToggleButton(mComponents, r, slotItems));
			mSlotMenus.back()->Initialize(mDevice);
			mSlotMenus.back()->SetEnabled(false);
			r.top = r.bottom;
			r.bottom = r.top + C_LABEL_HEIGHT;
		}

		r.left = C_LABEL_XOFFSET + C_LABEL_WIDTH + C_LABEL_MARGIN;
		r.right = r.left + C_MENU_WIDTH;
		r.top = C_LABEL_YOFFSET;
		r.bottom = r.top + C_LABEL_HEIGHT;

		std::vector<std::string> teamItems;
		teamItems.push_back("Team 1");
		teamItems.push_back("Team 2");

		for (unsigned int i = 0; i < mSession->GetSlotCount(); ++i)
		{
			mTeamMenus.push_back(new Components::ToggleButton(mComponents, r, teamItems));
			mTeamMenus.back()->Initialize(mDevice);
			mTeamMenus.back()->SetEnabled(false);
			r.top = r.bottom;
			r.bottom = r.top + C_LABEL_HEIGHT;
		}

		// Set initial focus
		mChat->SetFocus();
		mComponents->SetFocus();
	}

	void AbstractLobbyState::TeamChosen(int playerIndex, int teamIndex)
	{
		mSession->SendSetTeamMessage(playerIndex, teamIndex);
	}

	std::string AbstractLobbyState::GetCaptionForTeam(int team) const
	{
		switch (team)
		{
			case 0:
				return "Team 1";
			case 1:
				return "Team 2";
			default:
				return "No Team";
		}
	}

	std::string AbstractLobbyState::GetCaptionForSlot(Logic::Session::ClientSlot slotType) const
	{
		switch (slotType)
		{
			case Logic::Session::C_STATUS_LOCAL:
				return "Local";
			case Logic::Session::C_STATUS_OPEN:
				return "Network";
			default:
				return "Invalid";
		}
	}

	void AbstractLobbyState::HandleMenus()
	{
		for (int i = 0; i < mSession->GetSlotCount(); ++i)
		{
			if (mSlotMenus[i]->GetAndResetToggleStatus())
			{
				SlotChosen(i, mSlotMenus[i]->GetCurrentValue());
			}

			if (mTeamMenus[i]->GetAndResetToggleStatus())
			{
				TeamChosen(i, mTeamMenus[i]->GetCurrentValue());
			}
		}

		
	}
}
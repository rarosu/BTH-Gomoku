#ifndef ABSTRACT_LOBBY_STATE_HPP
#define ABSTRACT_LOBBY_STATE_HPP

#include "ApplicationState.hpp"
#include "Session.hpp"

#include "ComponentGroup.hpp"
#include "ChatConsole.hpp"
#include "TextButton.hpp"
#include "Label.hpp"
#include "ToggleButton.hpp"
#include "GameFont.hpp"		// DEBUG

namespace State
{
	class AbstractLobbyState : public ApplicationState, public Logic::SessionNotificationInterface, public Components::ChatInputReceiver
	{
	public:
		AbstractLobbyState(StateID id, ID3D10Device* device);
		~AbstractLobbyState() throw();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();

		/**
			Called when a line has been entered in the chat
		*/
		void ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message, Logic::PlayerID target, Network::Recipient::Recipient recipient);
		
		/**
			Sent when a chat message is received from the network.
		*/
		void ReceiveChatMessage(const std::string& message, Network::Recipient::Recipient recipient, Logic::PlayerID sourceID);

		void PlacePiece(Logic::PlayerID id, const Logic::Cell& cell);

		/**	
			Set the team of the given player.
		*/
		void SetTeam(Logic::PlayerID id, Logic::Team team);

		/**
			Sent when the game is over and has been won.

			This can, of course, not happen in the lobby, thus we are asserting against it.
		*/
		void GameOver(Logic::PlayerID winningPlayer);

		/**
			Sent when a player has connected.
		*/
		void PlayerConnected(Logic::PlayerID id);
		
		/**
			Sent when a player disconnects - the name is sent along since the player is
			removed from the player list.
		*/
		void PlayerDisconnected(Logic::PlayerID id, const std::string& name, Network::RemovePlayerReason::RemovePlayerReason reason);
	
		/**
			Highlight a cell for your teammate. 
		*/
		void SetHighlightedCell(const Logic::Cell& cell, int highlightType);
	protected:
		void SetSession(Logic::Session* session);

		virtual void AppendClientConnected(Logic::PlayerID player) {}
		virtual void AppendClientDisconnected(Logic::PlayerID player) {}
		virtual void AppendStatePushed() {}
		virtual void AppendStatePopped() {}
		virtual void AppendComponents() {}
		virtual void AppendUpdate() {}
		virtual void SlotChosen(int playerIndex, int slotIndex) {}


		ID3D10Device*							mDevice;

		Components::ComponentGroup*				mComponents;
		Components::TextButton*					mCancelButton;
		Components::ChatConsole*				mChat;
		std::vector<Components::Label*>			mPlayerLabels;
		std::vector<Components::ToggleButton*>	mTeamMenus;
		std::vector<Components::ToggleButton*>	mSlotMenus;

		std::string GetCaptionForTeam(int team) const;
		std::string GetCaptionForSlot(Logic::Session::ClientSlot slotType) const;
	private:
		Sprite*									mBackground;
		Logic::Session*							mSession;
		GameFont*								mDefaultFont;	//DEBUG

		void CreateComponents();
		void TeamChosen(int playerIndex, int teamIndex);
		void HandleMenus();
	};
}

#endif
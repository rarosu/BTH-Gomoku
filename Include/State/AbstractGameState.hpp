#ifndef ABSTRACT_GAME_STATE_HPP
#define ABSTRACT_GAME_STATE_HPP

#include "ApplicationState.hpp"
#include "Session.hpp"

#include "Scene.hpp"
#include "ComponentGroup.hpp"
#include "ChatConsole.hpp"
#include "PieMenu.hpp"
#include "ClickMenu.hpp"
#include "PlayerList.hpp"

#include "Sprite.hpp"

namespace State
{
	class AbstractGameState : public ApplicationState, public Logic::SessionNotificationInterface, public Components::ChatInputReceiver
	{
	public:
		AbstractGameState(StateID id, ID3D10Device* device);
		~AbstractGameState() throw();

		void OnStatePushed();
		void OnStatePopped();

		void OnResize();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		/**
			Called when a line has been entered in the chat
		*/
		void ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message);
		
		/**
			Sent when a chat message is received from the network.
		*/
		void ReceiveChatMessage(const std::string& message, Logic::PlayerID sourceID);

		/**
			Sent when the game is over and has been won.
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
	protected:
		void SetSession(Logic::Session* session);

		virtual void OnConnectionFailure() {}
		virtual void InitializeGame() {}
		virtual void EndGame() {}
		virtual bool CanSendChatMessage() const { return true; }

		void SetChatName(const std::string& name);

		Components::ChatConsole* mChat;
	private:
		ID3D10Device* mDevice;


		// The session, containing the logical grid and markers, and the connection
		// between server/client.
		Logic::Session* mSession;


		// The components of the game, including the actual scene which displays the
		// grid and all the markers.
		Components::ComponentGroup*	mComponents;
		Scene* mScene;
		Components::ClickMenu* mGameMenu;
		Components::PlayerList*	mPlayerList;

		// Indicates that either the game is over, or a disconnect happened.
		bool mGameOver;
		


		void CreateComponents();
		float GetAspectRatio();
	};
}

#endif
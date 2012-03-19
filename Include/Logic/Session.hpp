#ifndef SESSION_HPP
#define SESSION_HPP

#include <vector>
#include "GameTime.hpp"
#include "Player.hpp"
#include "Grid.hpp"
#include "ChatMessage.hpp"
#include "RemovePlayerMessage.hpp"

namespace Logic
{
	/**
		An interface for receiving general session notifications
	*/
	class SessionNotificationInterface
	{
	public:
		/**
			Sent when a chat message is received from the network.
		*/
		virtual void ReceiveChatMessage(const std::string& message, PlayerID sourceID) = 0;

		/**
			Sent when the game is over and has been won.
		*/
		virtual void GameOver(PlayerID winningPlayer) = 0;

		/**
			Sent when a player has connected.
		*/
		virtual void PlayerConnected(PlayerID id) = 0;
		
		/**
			Sent when a player disconnects - the name is sent along since the player is
			removed from the player list.
		*/
		virtual void PlayerDisconnected(PlayerID id, const std::string& name, Network::RemovePlayerReason::RemovePlayerReason reason) = 0;
	};



	/**
		A session holding the state of one game
	*/
	class Session
	{
	public:
		Session(unsigned int playerCount);
		virtual ~Session() throw();

		/**
			Getters
		*/
		unsigned int GetPlayerCount() const;
		unsigned int GetSlotCount() const;
		unsigned int GetCurrentPlayer() const;
		std::string GetPlayerName(PlayerID index) const;

		/**
			Check whether it is a local player's turn
		*/
		virtual bool IsLocalPlayerTurn() const = 0;

		/**
			Set/Get the session notification interface, where we send
			notifications on generic session events.
		*/
		void SetSessionNotifiee(SessionNotificationInterface* notifiee);
		const SessionNotificationInterface* GetSessionNotifiee() const;

		/**
			Get the current state of the grid
		*/
		const Grid& GetGrid() const;

		/**
			Update the logic in this session
		*/
		virtual void Update(const GameTime& gameTime) = 0;

		/**
			Send a chat message to the specified target player

			If broadcast or team is used, targetID is ignored.
		*/
		virtual void SendChatMessage(const std::string& message, PlayerID targetID, Network::Recipient::Recipient recipient) = 0;

		/**
			Place a piece
		*/
		virtual void SendPlacePieceMessage(const Logic::Cell& cell) = 0;

		/**
			Check whether there exists an open slot, in which case not all slots have been filled or a player has left the game.
		*/
		bool HasOpenSlot() const;

		/**
			Get the winning player (or C_PLAYER_NONE if no player has won yet)

			This will only return the winner if the mGameWon flag has been set, in which case
			the winner has been synchronized with all clients.
		*/
		PlayerID GetWinner() const;
	protected:
		SessionNotificationInterface* mSessionNotifiee;

		PlayerID mWinner;					// Holds the winner's player ID, if the game has been won and all clients has been notified of it. Holds C_PLAYER_NONE otherwise.
		Grid mGrid;							// Holds the virtual grid and all the markers on it
		std::vector<Player*> mPlayers;		// Holds all players in the game, their names, team and marker type. If a player is NULL, it is an open/local slot.
		PlayerID mCurrentPlayer;			// Holds the current player
	};
}

#endif
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
		virtual void ReceiveChatMessage(const std::string& message, Network::Recipient::Recipient recipient, PlayerID sourceID) = 0;

		/**
			Put a piece on the board and perform actions connected to this.
		*/

		virtual void PlacePiece(PlayerID id, const Cell& cell) = 0;

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
		typedef int ClientSlot;
		static const ClientSlot C_STATUS_REMOTE = -1;
		static const ClientSlot C_STATUS_OPEN = -2;
		static const ClientSlot C_STATUS_LOCAL = -3;

		Session(unsigned int playerCount);
		virtual ~Session() throw();

		/**
			Getters
		*/
		unsigned int GetPlayerCount() const;
		unsigned int GetSlotCount() const;
		unsigned int GetCurrentPlayer() const;
		std::string GetPlayerName(PlayerID index) const;
		PlayerID GetPlayerByName(const std::string& name) const;

		/**
			Get the team of the given player, or C_TEAM_NONE if not chosen yet.
		*/
		Team GetPlayerTeam(PlayerID index) const;

		/**
			Get the teammate of a specific player, or C_PLAYER_NONE if none exists.
		*/
		PlayerID GetTeamMate(PlayerID index) const;

		/**
			Get the nth opponent to the given player.
		*/
		PlayerID GetOpponent(PlayerID index, int nth) const;

		/**
			Make sure all teams are valid.
		*/
		bool AreTeamsValid() const;
		


		/**
			Check whether it is a local player's turn
		*/
		virtual bool IsLocalPlayerTurn() const = 0;

		/**
			Check whether a specific player is local or not.
		*/
		virtual bool IsLocalPlayer(PlayerID index) const = 0;

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
			Change a player's team.
		*/
		virtual void SendSetTeamMessage(PlayerID player, Team team) = 0;

		/**
			Check whether there exists an open slot, in which case not all slots have been filled or a player has left the game.
		*/
		bool HasOpenSlot() const;

		/**
			Get the type of a slot
		*/
		virtual ClientSlot GetSlotType(PlayerID playerID) const = 0;

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
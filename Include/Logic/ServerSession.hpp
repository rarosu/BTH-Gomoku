#ifndef SERVERSESSION_HPP
#define SERVERSESSION_HPP

#include <vector>
#include <map>
#include <string>
#include "Session.hpp"
#include "Server.hpp"
#include "Ruleset.hpp"
#include "Player.hpp"
#include "GameTime.hpp"
#include "ChatMessage.hpp"

namespace Logic
{
	/**
		When creating a game, a server session should be created, which will
		listen for incoming connections and check game logic.
	*/
	class ServerSession : public Session, public Network::ServerEventInterface
	{
	public:
		ServerSession(Network::Server* server, const std::string& adminName, Ruleset* ruleset);
		~ServerSession() throw();

		/**
			Check if the current player is one without a client, thus
			a local player.
		*/
		bool IsLocalPlayerTurn() const;

		/**
			Check whether a specific player is local or not.
		*/
		bool IsLocalPlayer(PlayerID index) const;

		/**
			Getters
		*/
		unsigned short GetPort() const;
		const Ruleset* GetRuleset() const;

		/**
			Check if the game is over, and return the winner in such case.

			If the game has not been won, return C_PLAYER_NONE.
		*/
		PlayerID GetWinner() const;

		/**
			Update the server session (listen to all clients, handle messages, listen for stay alive messages)
		*/
		void Update(const GameTime& gameTime);

		/**
			Send a chat message to the specified target player

			If broadcast or team is used, targetID is ignored.
		*/
		void SendChatMessage(const std::string& message, PlayerID targetID, Network::Recipient::Recipient recipient);

		/**
			Send a place piece message.
		*/
		void SendPlacePieceMessage(const Logic::Cell& cell);

		/**
			Send start game message
		*/
		void SendStartMessage();

		/**	
			Set the team of a local player.
		*/
		void SetLocalPlayerTeam(PlayerID playerID, Team team);

		/**
			Implemented from ServerEventInterface
		*/
		void ClientConnected(Network::Slot slot);
		void ClientDisconnected(Network::Slot slot);
	private:		
		typedef int ClientSlot;
		typedef std::map<PlayerID, ClientSlot> SlotMap;
		typedef std::map<ClientSlot, float> TimeoutMap;

		static const float C_TIMEOUT;
		static const ClientSlot C_STATUS_OPEN = -1;
		static const ClientSlot C_STATUS_LOCAL = -2;

		SlotMap mPlayerClients;								// Associates a player with a given client (or determines if the slot is open/local)
		std::vector<ClientSlot> mPendingClients;			// Holds all the pending clients that we're awaiting a join message from
		std::vector<ClientSlot> mClientsToRemove;			// Holds all the clients that are to be disconnected
		TimeoutMap mClientTimeout;							// Holds the timeout values for all clients

		Ruleset* mRuleset;									// The ruleset determines all the rules for the game
		Network::Server* mServer;							// The server holds all network clients, and queues their messages

		PlayerID GetPlayerSlot(ClientSlot slot) const;
		void HandleJoinMessage(Network::Slot clientSlot, const std::string& name);
		void HandleChatMessage(PlayerID sourceID, PlayerID targetID, Network::Recipient::Recipient recipient, const std::string& message);

		bool CheckAndHandleWin();
	};
}
#endif
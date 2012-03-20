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
			Get the type of a slot
		*/
		ClientSlot GetSlotType(PlayerID playerID) const;

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
			Set the team of a player, and notify about the event.
		*/
		void SendSetTeamMessage(PlayerID player, Team team);

		/**
			Send start game message
		*/
		void SendStartMessage();

		/**
			Boot a player
		*/
		void BootPlayer(PlayerID playerID);

		/**
			Set a slot type
		*/
		void SetSlotType(PlayerID playerSlot, ClientSlot status);

		/**
			Check if we can start a game.

			This can only happen, when we don't have any open slots (all network slots are filled)
			and teams are in order.
		*/
		bool CanStartGame() const;

		/**
			Implemented from ServerEventInterface
		*/
		void ClientConnected(Network::Slot slot);
		void ClientDisconnected(Network::Slot slot);
	private:		
		typedef std::map<PlayerID, ClientSlot> SlotMap;
		typedef std::map<ClientSlot, float> TimeoutMap;

		static const float C_TIMEOUT;
		

		SlotMap mPlayerClients;								// Associates a player with a given client (or determines if the slot is open/local)
		std::vector<ClientSlot> mPendingClients;			// Holds all the pending clients that we're awaiting a join message from
		std::vector<ClientSlot> mClientsToRemove;			// Holds all the clients that are to be disconnected
		TimeoutMap mClientTimeout;							// Holds the timeout values for all clients

		Ruleset* mRuleset;									// The ruleset determines all the rules for the game
		Network::Server* mServer;							// The server holds all network clients, and queues their messages

		void AddPlayer(PlayerID id, ClientSlot slot, const std::string& name, Team team);

		PlayerID GetPlayerSlot(ClientSlot slot) const;
		void HandleJoinMessage(Network::Slot clientSlot, const std::string& name);
		void HandleChatMessage(PlayerID sourceID, PlayerID targetID, Network::Recipient::Recipient recipient, const std::string& message);

		bool CheckAndHandleWin();
	};
}
#endif
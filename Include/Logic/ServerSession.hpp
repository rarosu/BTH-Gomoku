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
			Getters
		*/
		unsigned short GetPort() const;
		const Ruleset* GetRuleset() const;

		/**
			Get the name of a certain player.
		*/
		std::string GetPlayerName(unsigned int playerSlot) const;

		/**
			Update the session
		*/
		void Update(const GameTime& gameTime);

		/**
			Implemented from ServerEventInterface
		*/
		void ClientConnected(Network::Slot slot);
		void ClientDisconnected(Network::Slot slot);
	private:		
		typedef int PlayerSlot;
		typedef int ClientSlot;
		typedef std::map<PlayerSlot, ClientSlot> SlotMap;
		typedef std::map<ClientSlot, float> TimeoutMap;

		static const float C_TIMEOUT;
		static const ClientSlot C_STATUS_OPEN = -1;
		static const ClientSlot C_STATUS_LOCAL = -2;
		static const PlayerSlot C_INVALID_PLAYER = -1;

		SlotMap mPlayerClients;								// Associates a player with a given client (or determines if the slot is open/local)
		std::vector<ClientSlot> mPendingClients;			// Holds all the pending clients that we're awaiting a join message from
		std::vector<ClientSlot> mClientsToRemove;			// Holds all the clients that are to be disconnected
		TimeoutMap mClientTimeout;							// Holds the timeout values for all clients

		Ruleset* mRuleset;
		Network::Server* mServer;

		PlayerSlot GetPlayerSlot(ClientSlot slot) const;
		void HandleJoinMessage(Network::Slot clientSlot, const std::string& name);
	};
}
#endif
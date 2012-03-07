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
			Update the session
		*/
		void Update(const GameTime& gameTime);

		/**
			Implemented from ServerEventInterface
		*/
		void ClientConnected(Network::Slot slot);
		void ClientDisconnected(Network::Slot slot);
	private:
		typedef std::map<int, float> TimeoutMap;

		static const float C_TIMEOUT;

		Ruleset* mRuleset;

		Network::Server* mServer;
		TimeoutMap mTimeoutCounters;
		std::vector<int> mClientsToBeRemoved;

		void HandleJoinMessage(Network::Slot clientSlot, const std::string& name);
	};
}
#endif
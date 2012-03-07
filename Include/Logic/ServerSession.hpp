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
		void ClientConnected(int slot);
		void ClientDisconnected(int slot);
	private:
		static const float C_TIMEOUT;

		typedef std::map<int, float> TimeoutMap;

		Network::Server* mServer;
		TimeoutMap mTimeoutCounter;
		std::vector<int> mClientsToBeRemoved;

		Ruleset* mRuleset;
	};
}
#endif
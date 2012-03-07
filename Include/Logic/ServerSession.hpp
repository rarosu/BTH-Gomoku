#ifndef SERVERSESSION_HPP
#define SERVERSESSION_HPP

#include <vector>
#include <string>
#include "Session.hpp"
#include "Server.hpp"
#include "Ruleset.hpp"
#include "Player.hpp"

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

		unsigned short GetPort() const;
		const Ruleset* GetRuleset() const;


		/**
			Implemented from ServerEventInterface
		*/
		void ClientConnected(int slot);
		void ClientDisconnected(int slot);
	private:
		Network::Server* mServer;

		Ruleset* mRuleset;
	};
}
#endif
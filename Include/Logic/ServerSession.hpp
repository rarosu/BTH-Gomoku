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
		What needs to be filled in before a server
		can be created and setup to listen.
	*/
	struct ServerParameters
	{
		ServerParameters() : mPort(0) {}
		ServerParameters(unsigned short port, Ruleset* ruleset, const std::string& adminName)
			: mPort(port), mRuleset(ruleset), mAdminName(adminName) {}

		unsigned short mPort;
		Ruleset* mRuleset;
		std::string mAdminName;
	};

	/**
		When creating a game, a server session should be created, which will
		listen for incoming connections and check game logic.
	*/
	class ServerSession : public Session
	{
	public:
		ServerSession(const ServerParameters& parameters);
		~ServerSession() throw();
	private:
		Network::Server mServer;

		Ruleset* mRuleset;
	};
}
#endif
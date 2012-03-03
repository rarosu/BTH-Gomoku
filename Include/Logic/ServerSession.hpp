#ifndef SERVERSESSION_HPP
#define SERVERSESSION_HPP

#include <vector>
#include "Session.hpp"
#include "Player.hpp"

namespace Logic
{
	class ServerSession : public Session
	{
	public:
		ServerSession();
		~ServerSession() throw();


	private:
		std::vector<Player> mPlayers;
	};
}
#endif
#ifndef SESSION_HPP
#define SESSION_HPP

#include <vector>
#include "Player.hpp"

namespace Logic
{
	class Session
	{
	public:
		Session(unsigned int playerCount);
		virtual ~Session() throw();
	protected:
		std::vector<Player*> mPlayers;
	};
}

#endif
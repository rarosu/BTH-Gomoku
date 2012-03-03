#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "PlayerController.hpp"

namespace Logic
{
	typedef int PlayerID;

	const PlayerID C_PLAYER_NONE = 0;

	
	class Player
	{
	public:
		
	private:
		std::string mName;
		unsigned int mTeam;
		unsigned int mMarker;

		PlayerController* mController;
	};
}

#endif

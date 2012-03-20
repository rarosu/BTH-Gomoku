#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <cassert>
#include "Globals.hpp"
#include "Cell.hpp"

namespace Logic
{
	typedef int PlayerID;
	typedef int Team;

	const PlayerID C_PLAYER_NONE = -1;
	const Team C_TEAM_NONE = -1;
	
	class Player
	{
	public:
		Player(const std::string& name, Team team);
		~Player() throw();
		
		void SetName(const std::string& name);
		void SetTeam(Team team);

		const std::string& GetName() const { return mName; }
		Team GetTeam() const { return mTeam; }
	private:
		std::string mName;
		Team mTeam;

		Player(const Player& copy) { assert(false); }
		Player& operator=(const Player& copy) { assert(false); }
	};
}

#endif

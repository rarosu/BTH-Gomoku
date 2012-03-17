#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <cassert>
#include "Globals.hpp"
#include "Cell.hpp"

namespace Logic
{
	typedef int PlayerID;

	const PlayerID C_PLAYER_NONE = -1;

	
	class Player
	{
	public:
		typedef unsigned int Team;
		typedef unsigned int MarkerType;

		Player(const std::string& name, Team team, MarkerType markerType);
		~Player() throw();
		
		void SetName(const std::string& name);
		void SetTeam(Team team);
		void SetMarkerType(MarkerType markerType);

		const std::string& GetName() const { return mName; }
		Team GetTeam() const { return mTeam; }
		MarkerType GetMarkerType() const { return mMarker; }
	private:
		std::string mName;
		Team mTeam;
		MarkerType mMarker;

		Player(const Player& copy) { assert(false); }
		Player& operator=(const Player& copy) { assert(false); }
	};
}

#endif

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "Globals.hpp"
#include "PlayerController.hpp"
#include "Cell.hpp"

namespace Logic
{
	typedef int PlayerID;

	const PlayerID C_PLAYER_NONE = 0;

	
	class Player
	{
	public:
		typedef unsigned int Team;
		typedef unsigned int MarkerType;

		Player();
		~Player() throw();


		void SetController(PlayerController* controller);
		
		void SetName(const std::string& name);
		void SetTeam(Team team);
		void SetMarkerType(MarkerType markerType);

		const std::string& GetName() const { return mName; }
		Team GetTeam() const { return mTeam; }
		MarkerType GetMarkerType() const { return mMarker; }


		/**
			Decide a move, and output the coordinates in the cell parameter.

			This will delegate to the controller, which will decide how to best make a move.
			If a move has not yet been decided (e.g. player is thinking, waiting for message, etc.)
			this method will return false, and cell will be (0, 0).

			If controller is NULL, this method always returns false.
		*/
		bool MakeMove(Cell& cell) const;
	private:
		std::string mName;
		Team mTeam;
		MarkerType mMarker;

		PlayerController* mController;
	};
}

#endif

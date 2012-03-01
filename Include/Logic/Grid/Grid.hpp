#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <map>
#include "Player.hpp"
#include "Cell.hpp"

namespace Logic
{
	/**
		A grid holds the instances of all markers and all the rows
		of a game.
	*/
	class Grid
	{
	public:
		typedef std::map<Cell, PlayerID> MarkerMap;
		
		/**
			Construct an empty grid
		*/
		Grid();
	
		/**
			For adding a new marker to the grid
		*/
		bool AddMarker(const Cell& cell, PlayerID player);
		
		/**
			For accessing the grid data
		*/
		const PlayerID GetMarkerInCell(const Cell& cell) const;
		const PlayerID GetMarkerInCell(int x, int y) const;

		/**
			For iterating through the markers
		*/
		MarkerMap::const_iterator GetMarkerMapStart() const;
		MarkerMap::const_iterator GetMarkerMapEnd() const;

		/**
			Get the leaderboard
		*/
		unsigned int GetLeadingCount() const;
		PlayerID GetLeadingPlayer() const;
	private:
		/**
			Keep going in one direction, counting all cells of the same player
		*/
		unsigned int CountMarkersInRow(const Cell& cell, Direction::Direction direction) const;

		/**
			The leaderboard
		*/
		PlayerID mLeadingPlayer;
		unsigned int mLeadingCount;

		/**
			The actual markers
		*/
		MarkerMap mMarkers;
	};
}

#endif

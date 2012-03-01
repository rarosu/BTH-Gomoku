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
		typedef std::vector<Cell> Row;
		
		/**
			Construct an empty grid
		*/
		Grid();
	
		/**
			For adding a new marker to the grid
		*/
		bool AddMarker(const Cell& cell, PlayerID player);

		/**
			For removing markers from the grid
		*/
		void RemoveMarker(const Cell& cell);
		
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
		const Row& GetLeadingRow() const;
		PlayerID GetLeadingPlayer() const;
	private:
		/**
			Keep going in one direction, counting all cells of the same player
		*/
		void CountMarkersInRow(std::vector<Cell>& row, PlayerID player, const Cell& cell, Direction::Direction direction) const;

		/**
			The leaderboard
		*/
		std::vector<Cell> mLeadingRow;

		/**
			The actual markers
		*/
		MarkerMap mMarkers;
	};
}

#endif

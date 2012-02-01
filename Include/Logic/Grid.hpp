#ifndef GRID_HPP
#define GRIP_HPP

#include <vector>
#include <map>
#include "Player.hpp"
#include "Cell.hpp"
#include "Row.hpp"

namespace Logic
{
	/**
		A grid holds the instances of all markers and all the rows
		of a game.
	*/
	class Grid
	{
	public:
		typedef std::map<Cell, Player> MarkerMap;
		typedef std::vector<Row> RowVector;
	
		bool AddMarker(const Cell& cell, Player player);
		
		const RowVector& GetRows() const;
		const Player GetMarkerInCell(const Cell& cell) const;
		const Player GetMarkerInCell(int x, int y) const;

		MarkerMap::const_iterator GetFirstOccupiedCell() const;
		MarkerMap::const_iterator GetMarkerMapEnd() const;
	private:
		MarkerMap mMarkers;
		RowVector mRows;
	};
}

#endif

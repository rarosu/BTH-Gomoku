#include "Grid.hpp"

namespace Logic
{
	Grid::Grid()
		: mLeadingPlayer(C_PLAYER_NONE),
		  mLeadingCount(0)	{}

	unsigned int Grid::CountMarkersInRow(const Cell& cell, Direction::Direction direction) const
	{
		Cell neighbour = cell.GetNeighbour(direction);
		if (GetMarkerInCell(neighbour) == GetMarkerInCell(cell))
			return 1 + CountMarkersInRow(neighbour, direction);
	}

	bool Grid::AddMarker(const Cell& cell, PlayerID player) 
	{
		// Can only add a marker if the cell is unoccupied
		if (GetMarkerInCell(cell) != C_PLAYER_NONE) 
			return false;
		mMarkers[cell] = player;
		
		// Check every direction
		for (int directionIterator = Direction::C_FIRST;
			 directionIterator < (Direction::C_COUNT - 1) / 2;
			 ++directionIterator)
		{
			Direction::Direction directions[2] = { static_cast<Direction::Direction>(directionIterator),
												   Direction::GetOpposite(static_cast<Direction::Direction>(directionIterator)) };

			unsigned int count = 1;
			for (int i = 0; i < 2; ++i)
			{
				count += CountMarkersInRow(cell.GetNeighbour(directions[i]), directions[i]);
			}

			if (count > mLeadingCount)
			{
				mLeadingPlayer = mMarkers[cell];
				mLeadingCount = count;
			}
		}

		return true;
	}
	
	const PlayerID Grid::GetMarkerInCell(const Cell& cell) const
	{
		MarkerMap::const_iterator it = mMarkers.find(cell);

		if (it == mMarkers.end())
			return C_PLAYER_NONE;
		return it->second;
	}
	
	const PlayerID Grid::GetMarkerInCell(int x, int y) const
	{
		return GetMarkerInCell(Cell(x, y));
	}

	Grid::MarkerMap::const_iterator Grid::GetMarkerMapStart() const
	{
		return mMarkers.begin();
	}

	Grid::MarkerMap::const_iterator Grid::GetMarkerMapEnd() const
	{
		return mMarkers.end();
	}
	
}

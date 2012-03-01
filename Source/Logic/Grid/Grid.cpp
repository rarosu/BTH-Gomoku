#include "Grid.hpp"

namespace Logic
{
	Grid::Grid() {}

	void Grid::CountMarkersInRow(std::vector<Cell>& row, PlayerID player, const Cell& cell, Direction::Direction direction) const
	{
		if (GetMarkerInCell(cell) != player)
			return;
		
		row.push_back(cell);
		CountMarkersInRow(row, player, cell.GetNeighbour(direction), direction);
	}

	bool Grid::AddMarker(const Cell& cell, PlayerID player) 
	{
		// Can only add a marker if the cell is unoccupied
		if (GetMarkerInCell(cell) != C_PLAYER_NONE) 
			return false;
		mMarkers[cell] = player;
		
		// Check every direction
		for (int directionIterator = Direction::C_FIRST;
			 directionIterator <= (Direction::C_COUNT - 1) / 2;
			 ++directionIterator)
		{
			// Enumerate the direction and its opposite
			Direction::Direction directions[2] = { static_cast<Direction::Direction>(directionIterator),
												   Direction::GetOpposite(static_cast<Direction::Direction>(directionIterator)) };

			// Count all markers lying in a row in this alignment
			std::vector<Cell> row;
			row.push_back(cell);
			for (int i = 0; i < 2; ++i)
			{
				CountMarkersInRow(row, GetMarkerInCell(cell), cell.GetNeighbour(directions[i]), directions[i]);
			}

			// If the counted row was larger than the currently longest row, replace it
			if (row.size() > mLeadingRow.size())
			{
				mLeadingRow = row;
			}
		}

		return true;
	}

	void Grid::RemoveMarker(const Cell& cell)
	{
		if (GetMarkerInCell(cell) != C_PLAYER_NONE)
		{
			mMarkers[cell] = C_PLAYER_NONE;

			// We cannot know anything about the rows now, 
			// so just clear it
			mLeadingRow.clear();
		}
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

	const Grid::Row& Grid::GetLeadingRow() const
	{
		return mLeadingRow;
	}

	PlayerID Grid::GetLeadingPlayer() const
	{
		if (mLeadingRow.size() == 0)
			return C_PLAYER_NONE;
		return GetMarkerInCell(mLeadingRow[0]);
	}
	
}

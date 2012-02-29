#include "Grid.hpp"

namespace Logic
{
	bool Grid::AddMarker(const Cell& cell, PlayerID player) 
	{
		// Can only add a marker if the cell is unoccupied
		if (mMarkers[cell] != K_PLAYER_NONE) 
			return false;
		
		// Check every neighbour of the given cell...
		for (int directionIterator = Direction::Up;
			 directionIterator != Direction::UpLeft;
			 ++directionIterator)
		{
			Direction::Direction direction = (Direction::Direction) directionIterator;
			Cell neighbour = cell.GetNeighbour(direction);
			
			// If this cell and its neighbour are of the same player...
			if (mMarkers[neighbour] == mMarkers[cell])
			{
				// Check every row in the Grid, to see if we find one which the neighbour
				// belongs to and the given cell could be added to.
				bool foundRow = false;
				for (RowVector::iterator rowIterator = mRows.begin();
					 rowIterator != mRows.end();
					 rowIterator++)
				{
					if (rowIterator->Contains(neighbour))
					{
						if (rowIterator->CanAddMarker(cell))
						{
							rowIterator->AddMarker(cell);
							foundRow = true;

							break;
						}
					}
				}
				
				
				// See if we have two cells that are neighbours, of the same player and
				// are not part of a row with their alignment. In that case, we'll need to
				// create a new row.
				if (!foundRow)
				{
					Row newRow(this);
					newRow.AddMarker(cell);
					newRow.AddMarker(neighbour);
					
					mRows.push_back(newRow);
				}
			}
		}
		
		return true;
	}
	
	const Grid::RowVector& Grid::GetRows() const
	{
		return mRows;
	}
	
	const PlayerID Grid::GetMarkerInCell(const Cell& cell) const
	{
		return mMarkers.find(cell)->second;
	}
	
	const PlayerID Grid::GetMarkerInCell(int x, int y) const
	{
		return mMarkers.find(Cell(x, y))->second;
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

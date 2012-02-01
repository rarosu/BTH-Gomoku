#include "Grid.hpp"

namespace model
{
	bool Grid::AddMarker(const Cell& cell, Player player) 
	{
		if (mMarkers[cell] != K_PLAYER_NONE) return false;
		
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
					// Make sure the neighbour belongs to the row...
					if (rowIterator->Contains(neighbour))
					{
						// Make sure the given cell is aligned with the row...
						if (rowIterator->IsAligned(cell))
						{
							// Add the marker to this row!
							rowIterator->AddMarker(cell);
							foundRow = true;
							
							// No need to search for more rows in this direction
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
	
	const Player Grid::GetMarkerInCell(const Cell& cell) const
	{
		return mMarkers.find(cell)->second;
	}
	
	const Player Grid::GetMarkerInCell(int x, int y) const
	{
		return mMarkers.find(Cell(x, y))->second;
	}
	
}

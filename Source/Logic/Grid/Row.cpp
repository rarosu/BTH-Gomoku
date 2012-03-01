#include "Row.hpp"
#include "Grid.hpp"
#include <algorithm>
#include <cassert>

namespace Logic
{
	Row::Row(const Cell& cell, Grid* grid) 
		: mGrid(grid), 
		  mAlignment(Alignment::None), 
		  mPlayer(C_PLAYER_NONE) 
	{
		assert(grid->GetMarkerInCell(cell) != C_PLAYER_NONE);

		mCells.push_back(cell);
		mPlayer = grid->GetMarkerInCell(cell);
	}
	
	bool Row::IsAligned(const Cell& cell) const
	{
		if (Length() == 0) return true;
		
		for (CellVector::const_iterator it = mCells.begin();
			 it != mCells.end();
			 it++)
		{
			if (it->IsNeighbour(cell))
			{
				if (mAlignment == Alignment::None ||
					mAlignment == it->GetAlignmentToNeighbour(cell))
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	bool Row::Contains(const Cell& cell) const
	{
		for (CellVector::const_iterator it = mCells.begin();
			 it != mCells.end();
			 it++)
		{
			if (*it == cell)	
				return true;
		}
		
		return false;
	}

	bool Row::IsSamePlayer(const Cell& cell) const
	{
		PlayerID marker = mGrid->GetMarkerInCell(cell);
		
		if (marker == C_PLAYER_NONE)
			return false;
		if (mCells.size() == 0)
			return true;

		return marker == mGrid->GetMarkerInCell(mCells[0]);
	}

	bool Row::CanAddMarker(const Cell& cell) const
	{
		if (Contains(cell))
			return false;
		if (!IsSamePlayer(cell))
			return false;
		if (!IsAligned(cell))
			return false;
		return true;
	}
	
	/*
	void Row::AddMarker(const Cell& cell)
	{
		if (!CanAddMarker(cell))
			return;
		
		mCells.push_back(cell);
		
		if (mAlignment == Alignment::None &&
			Length() >= 2)
		{
			mAlignment = mCells[0].GetAlignmentToNeighbour(mCells[1]);
		}
	}
	*/

	bool Row::Merge(const Row& row)
	{
		// Check if these rows share a neighbour
		bool breakFlag = false;
		Alignment::Alignment alignment = Alignment::None;
		for (int i = 0; i < Length(); ++i)
		{
			for (int k = 0; k < row.Length(); ++k)
			{
				if (mCells[i].IsNeighbour(row.mCells[k]))
				{
					Alignment::Alignment alignmentToNeighbour = mCells[i].GetAlignmentToNeighbour(row.mCells[k]);
					if (mAlignment == Alignment::None || mAlignment == alignmentToNeighbour)
					{
						alignment = alignmentToNeighbour;
						breakFlag = true;
						break;
					}
				}
			}

			if (breakFlag)
				break;
		}

		// They share no neighbour, so merge is impossible
		if (alignment == Alignment::None)
			return false;

		// Merge the rows
		mAlignment = alignment;
		for (int i = 0; i < row.Length(); ++i)
		{
			mCells.push_back(row.mCells[i]);
		}

		return true;
	}
	
	int Row::Length() const
	{
		return mCells.size();
	}
}

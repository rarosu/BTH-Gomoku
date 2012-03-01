#include "Row.hpp"
#include "Grid.hpp"
#include <algorithm>
#include <cassert>

namespace Logic
{
	Row::Row(Grid* grid) : mGrid(grid), mAlignment(Alignment::None), mPlayer(C_PLAYER_NONE) {}
		
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
					it->GetAlignmentToNeighbour(cell))
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
			if (*it == cell)	return true;
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

	bool Row::Merge(const Row& row)
	{
		assert(row.Length() >= 2);
		assert(Length() >= 2);

		if (mAlignment != row.mAlignment)
			return false;
		if (mPlayer != row.mPlayer)
			return false;

		for (int i = 0; i < row.mCells.size(); ++i)
		{
			assert(mCells[i] != row.mCells[i]);
			mCells.push_back(row.mCells[i]);
		}

		return true;
	}
	
	int Row::Length() const
	{
		return mCells.size();
	}
}

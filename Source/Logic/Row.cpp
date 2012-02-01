#include "Row.hpp"
#include <algorithm>

namespace model
{
	Row::Row(Grid* grid) : mGrid(grid), mAlignment(Alignment::None), mPlayer(K_PLAYER_NONE) {}
		
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
	
	void Row::AddMarker(const Cell& cell)
	{
		if (Contains(cell)) return;
		if (!IsAligned(cell)) return;
		
		mCells.push_back(cell);
		
		if (mAlignment == Alignment::None &&
			Length() >= 2)
		{
			mAlignment = mCells[0].GetAlignmentToNeighbour(mCells[1]);
		}
	}
	
	int Row::Length() const
	{
		return mCells.size();
	}
}

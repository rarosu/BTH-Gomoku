#ifndef ROW_HPP
#define ROW_HPP

#include <vector>
#include "Player.hpp"
#include "Cell.hpp"

namespace Logic
{
	// Forward declare Grid, since we need to access it in the Row class
	class Grid;
	
	/**
		A row is a collection of 2 or more aligned markers of the same player.
	*/
	class Row
	{
	public:
		Row(Grid* grid);
		
		/**
			Check whether a given cell is aligned with this row -
			i.e. is a neighbour and lies in line with all the other
			cells.
			
			If this row has no cells yet, this will always return true.
		*/
		bool IsAligned(const Cell& cell) const;
		
		/**
			See if this row contains a given cell
		*/
		bool Contains(const Cell& cell) const;
		
		/**
			This method will check whether cell is aligned and of the
			right player - and if that is the case, add it to this row.
		*/
		void AddMarker(const Cell& cell);
		
		/**
			Get the number of markers in this row
		*/
		int Length() const;
	private:
		typedef std::vector<Cell> CellVector;
	
		Grid* mGrid;
		
		CellVector mCells;
		
		Alignment::Alignment mAlignment;
		Player mPlayer;
	};
}

#endif

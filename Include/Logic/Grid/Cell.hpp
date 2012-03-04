#ifndef CELL_HPP
#define CELL_HPP

#include "Direction.hpp"

namespace Logic
{
	typedef int CellIndex;
	
	struct Cell
	{
	public:
		int x;
		int y;
		
		Cell();
		Cell(int x, int y);
		
		/**
			Determine if two cells are adjacent to eachother.
		*/
		bool IsNeighbour(const Cell& cell) const;
		
		/**
			Return the coordinates of a neighbour in given direction.
			If direction is None, a copy of this cell will be returned.
		*/
		Cell GetNeighbour(Direction::Direction direction) const;
		
		/**
			Return the direction from this cell to an adjacent cell.
		*/
		Direction::Direction GetDirectionToNeighbour(const Cell& neighbour) const;
		
		/**
			Return the alignment between two cells.
		*/
		Alignment::Alignment GetAlignmentToNeighbour(const Cell& neighbour) const;
		
		/**
			Comparison operators
		*/
		bool operator==(const Cell& cell) const;
		bool operator!=(const Cell& cell) const;
		bool operator<(const Cell& cell) const;
	};
}

#endif

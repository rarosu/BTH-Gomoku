#include "Cell.hpp"
#include <cmath>

namespace Logic
{
	Cell::Cell() : x(0), y(0) {}
	
	Cell::Cell(int x, int y) 
	{
		this->x = x;
		this->y = y;
	}
	
	bool Cell::IsNeighbour(const Cell& cell) const
	{
		int dx = cell.x - x;
		int dy = cell.y - y;
		
		return std::abs(dx) == 1 || std::abs(dy) == 1;
	}
	
	Cell Cell::GetNeighbour(Direction::Direction direction) const
	{
		Cell neighbour = *this;
		
		switch (direction) 
		{
			case Direction::Up:
				++neighbour.y;
			break;
			
			case Direction::UpRight:
				++neighbour.x;
				++neighbour.y;
			break;
			
			case Direction::Right:
				++neighbour.x;
			break;
			
			case Direction::DownRight:
				++neighbour.x;
				--neighbour.y;
			break;
			
			case Direction::Down:
				--neighbour.y;
			break;
			
			case Direction::DownLeft:
				--neighbour.x;
				--neighbour.y;
			break;
			
			case Direction::Left:
				--neighbour.x;
			break;
			
			case Direction::UpLeft:
				--neighbour.x;
				++neighbour.y;
			break;
			
			case Direction::None:
				// Do nothing
			break;
		}
		
		return neighbour;
	}
	
	
	Direction::Direction Cell::GetDirectionToNeighbour(const Cell& neighbour) const
	{
		int dx = neighbour.x - x;
		int dy = neighbour.y - y;
		
		switch (dx)
		{
			case -1:
				switch (dy)
				{
					case -1:	return Direction::DownLeft;
					case 0:		return Direction::Left;
					case 1:		return Direction::UpLeft;
				}
			break;
			
			case 0:
				switch (dy)
				{
					case -1:	return Direction::Down;
					case 0:		return Direction::None;
					case 1:		return Direction::Up;
				}
			break;
			
			case 1:
				switch (dy)
				{
					case -1:	return Direction::DownRight;
					case 0:		return Direction::Right;
					case 1:		return Direction::UpRight;
				}
			break;			
		}

		return Direction::None;
	}
	
	bool Cell::operator==(const Cell& cell) const
	{
		return (x == cell.x) && (y == cell.y);
	}

	bool Cell::operator!=(const Cell& cell) const
	{
		return !(*this == cell);
	}

	bool Cell::operator<(const Cell& cell) const
	{
		if (x < cell.x) return true;
		if (x > cell.x) return false;
		if (y < cell.y) return true;
		return false;
	}
	
}

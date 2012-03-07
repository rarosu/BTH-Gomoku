#ifndef DIRECTION_HPP
#define DIRECTION_HPP

namespace Logic
{
	namespace Alignment
	{
		enum Alignment { None, Vertical, DiagonalLLToUR, Horizontal, DiagonalULToLR };
		const int C_COUNT = 5;
		const Alignment C_FIRST = Alignment::Vertical;
	}
	
	namespace Direction 
	{
		enum Direction { None, Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft };
		const int C_COUNT = 9;
		const Direction C_FIRST = Direction::Up;

		Direction GetOpposite(Direction direction);
	}
}

#endif

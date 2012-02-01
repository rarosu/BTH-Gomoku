#ifndef DIRECTION_HPP
#define DIRECTION_HPP

namespace Logic
{
	namespace Alignment
	{
		enum Alignment { None, Vertical, DiagonalLLToUR, Horizontal, DiagonalULToLR };
	}
	
	namespace Direction 
	{
		enum Direction { None, Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft };
	}
}

#endif

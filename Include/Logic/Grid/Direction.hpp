#ifndef DIRECTION_HPP
#define DIRECTION_HPP

namespace Logic
{
	namespace Direction
	{
		enum Direction { None, Up, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft };
	
		Direction GetOpposite(Direction direction);
	}

	
}

#endif
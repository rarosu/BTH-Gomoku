#include "Direction.hpp"

namespace Logic
{
	namespace Direction
	{
		Direction GetOpposite(Direction direction)
		{
			switch (direction)
			{
				case Up:		return Down;
				case UpRight:	return DownLeft;
				case Right:		return Left;
				case DownRight:	return UpLeft;
				case Down:		return Up;
				case DownLeft:	return UpRight;
				case Left:		return Right;
				case UpLeft:	return DownRight;
				default:		return None;
			}
		}	
	}
}
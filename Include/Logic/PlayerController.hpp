#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include "Cell.hpp"

namespace Logic
{
	class PlayerController
	{
	public:
		virtual ~PlayerController() throw() {}

		virtual bool MakeMove(Cell& cell) = 0;
	};
}

#endif
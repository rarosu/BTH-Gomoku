#include "StandardRuleset.hpp"
#include <cstdlib>

namespace Logic
{
	unsigned int StandardRuleset::GetPlayerCount() const
	{
		return 2;
	}

	unsigned int StandardRuleset::GetWinningRowLength() const
	{
		return 5;
	}


	unsigned int StandardRuleset::GetStartingPlayer() const
	{
		return rand() % GetPlayerCount();
	}

	unsigned int StandardRuleset::GetNextPlayer(unsigned int currentPlayer) const
	{
		currentPlayer++;
		currentPlayer %= GetPlayerCount();

		return currentPlayer;
	}

}
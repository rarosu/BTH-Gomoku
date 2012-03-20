#include "Ruleset2v2.hpp"
#include "Session.hpp"
#include <cstdlib>

namespace Logic
{
	unsigned int Ruleset2v2::GetPlayerCount() const
	{
		return 4;
	}

	unsigned int Ruleset2v2::GetWinningRowLength() const
	{
		return 4;
	}


	unsigned int Ruleset2v2::GetStartingPlayer(const Session* session) const
	{
		unsigned int startingPlayer = rand() % 4;
		mTurnOrder.push_back(startingPlayer);
		mTurnOrder.push_back(session->GetOpponent(startingPlayer, 0));
		mTurnOrder.push_back(session->GetTeamMate(startingPlayer));
		mTurnOrder.push_back(session->GetTeamMate(mTurnOrder[1]));

		return startingPlayer;
	}

	unsigned int Ruleset2v2::GetNextPlayer(unsigned int currentPlayer) const
	{
		return mTurnOrder[(++currentPlayer) % GetPlayerCount()];
	}

}
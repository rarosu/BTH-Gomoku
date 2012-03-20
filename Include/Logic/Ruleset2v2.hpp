#ifndef RULESET_2V2_HPP
#define RULESET_2V2_HPP

#include "Ruleset.hpp"
#include <vector>

namespace Logic
{
	class Ruleset2v2 : public Ruleset
	{
	public:
		unsigned int GetPlayerCount() const;
		unsigned int GetWinningRowLength() const;

		unsigned int GetStartingPlayer(const Session* session) const;
		unsigned int GetNextPlayer(unsigned int currentPlayer) const;
	private:
		mutable std::vector<unsigned int> mTurnOrder;
	};
}

#endif
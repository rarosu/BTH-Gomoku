#ifndef STANDARD_RULESET_HPP
#define STANDARD_RULESET_HPP

#include "Ruleset.hpp"

namespace Logic
{
	class StandardRuleset : public Ruleset
	{
	public:
		unsigned int GetPlayerCount() const;
		unsigned int GetWinningRowLength() const;

		unsigned int GetStartingPlayer(const Session* session) const;
		unsigned int GetNextPlayer(unsigned int currentPlayer) const;
	};
}

#endif
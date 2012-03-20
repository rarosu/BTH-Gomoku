#ifndef RULESET_2V2_HPP
#define RULESET_2V2_HPP

#include "Ruleset.hpp"
#include <vector>

namespace Logic
{
	class Session;

	class Ruleset2v2 : public Ruleset
	{
	public:
		Ruleset2v2(const Session* session);

		unsigned int GetPlayerCount() const;
		unsigned int GetWinningRowLength() const;

		unsigned int GetStartingPlayer() const;
		unsigned int GetNextPlayer(unsigned int currentPlayer) const;
	private:
		const Session* mSession;
		mutable std::vector<unsigned int> mTurnOrder;
	};
}

#endif
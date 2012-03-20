#ifndef RULESET_HPP
#define RULESET_HPP

namespace Logic
{
	class Session;

	class Ruleset
	{
	public:
		virtual unsigned int GetPlayerCount() const = 0;
		virtual unsigned int GetWinningRowLength() const = 0;

		virtual unsigned int GetStartingPlayer(const Session* session) const = 0;
		virtual unsigned int GetNextPlayer(unsigned int currentPlayer) const = 0;
	};
}

#endif
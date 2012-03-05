#ifndef RULESET_HPP
#define RULESET_HPP

namespace Logic
{
	class Ruleset
	{
	public:
		virtual unsigned int GetPlayerCount() const = 0;
		virtual unsigned int GetWinningRowLength() const = 0;
	};
}

#endif
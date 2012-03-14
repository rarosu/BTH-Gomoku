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

	class StandardRuleset : public Ruleset
	{
	public:
		unsigned int GetPlayerCount() const { return 4; }
		unsigned int GetWinningRowLength() const { return 5; }
	};
}

#endif
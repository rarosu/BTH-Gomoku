#ifndef GAMETIME_H
#define GAMETIME_H

#include "WinInclude.hpp"

struct Time
{
	double Milliseconds;
	double Seconds;
};

class GameTime
{
public:
	GameTime();
	~GameTime();
	void Update();
	const Time& GetTimeSinceLastTick() const;
	const Time& GetTimeSinceGameStart() const;

private:
	__int64 mPrevTimeStamp;
	__int64 mGameStartTime;

	double mMilliSecondsPerTick;

	Time mElapsedSinceStart;
	Time mElapsedSinceLastTick;
};

#endif
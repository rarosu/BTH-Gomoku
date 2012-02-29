#include "Session.hpp"
#include <cassert>

namespace Logic
{
	void Session::SetNumberOfPlayers(int count)
	{
		mPlayers.resize(count);
	}

	void Session::AddPlayer(int slot, const Player& player)
	{
		assert(slot < mPlayers.size());

		mPlayers[slot] = player;
	}
}
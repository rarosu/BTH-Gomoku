#include "Session.hpp"
#include "Globals.hpp"
#include <cassert>
#include <cstdlib>

namespace Logic
{
	Session::Session(unsigned int playerCount)
		: mSessionNotifiee(NULL)
		, mWinner(C_PLAYER_NONE)
		, mCurrentPlayer(C_PLAYER_NONE)
	{
		mPlayers.resize(playerCount);
		for (unsigned int i = 0; i < mPlayers.size(); ++i)
		{
			mPlayers[i] = NULL;
		}
	}

	Session::~Session() throw()
	{
		for (unsigned int i = 0; i < mPlayers.size(); ++i)
		{
			SafeDelete(mPlayers[i]);
		}
	}

	unsigned int Session::GetPlayerCount() const
	{
		int count = 0;
		
		for (int i = 0; i < mPlayers.size(); ++i)
			if (mPlayers[i] != NULL)
				++count;

		return count;
	}

	unsigned int Session::GetSlotCount() const
	{
		return mPlayers.size();
	}

	unsigned int Session::GetCurrentPlayer() const
	{
		return mCurrentPlayer;
	}

	std::string Session::GetPlayerName(PlayerID index) const
	{
		std::string result;
		if (index >= 0 && index < mPlayers.size())
			if (mPlayers[index] != NULL)
				result = mPlayers[index]->GetName();

		return result;
	}

	PlayerID Session::GetPlayerByName(const std::string& name) const
	{
		PlayerID id = C_PLAYER_NONE;
		for (PlayerID i = 0; i < mPlayers.size(); ++i)
		{
			if (mPlayers[i]->GetName() == name)
			{
				id = i;
				break;
			}
		}

		return id;
	}

	const Grid& Session::GetGrid() const
	{
		return mGrid;
	}


	void Session::SetSessionNotifiee(SessionNotificationInterface* notifiee)
	{
		mSessionNotifiee = notifiee;
	}

	const SessionNotificationInterface* Session::GetSessionNotifiee() const
	{
		return mSessionNotifiee;
	}

	bool Session::HasOpenSlot() const
	{
		for (unsigned int i = 0; i < mPlayers.size(); ++i)
		{
			if (mPlayers[i] == NULL)
				return true;
		}

		return false;
	}

	bool Session::IsOpenSlot(PlayerID playerID) const
	{
		return mPlayers[playerID] == NULL;
	}

	PlayerID Session::GetWinner() const
	{
		return mWinner;
	}
}
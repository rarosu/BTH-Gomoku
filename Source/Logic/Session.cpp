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

	Team Session::GetPlayerTeam(PlayerID index) const
	{
		Team team = C_TEAM_NONE;

		if (mPlayers[index] != NULL)
		{
			team = mPlayers[index]->GetTeam();
		}

		return team;
	}

	PlayerID Session::GetTeamMate(PlayerID index) const
	{
		PlayerID player = C_PLAYER_NONE;
		Team team = GetPlayerTeam(index);

		if (team != C_TEAM_NONE)
		{
			for (unsigned int i = 0; i < mPlayers.size(); ++i)
			{
				if (i != index && mPlayers[i]->GetTeam() == team)
				{
					player = i;
					break;
				}
			}
		}

		return player;
	}

	PlayerID Session::GetOpponent(PlayerID index, int nth) const
	{
		Team team = GetPlayerTeam(index);
		if (mPlayers.size() > 2)
		{
			if (team != C_TEAM_NONE)
			{
				nth %= 2; 

				int count = 0;
				for (unsigned int i = 0; i < mPlayers.size(); ++i)
				{
					if (mPlayers[i] != NULL && mPlayers[i]->GetTeam() != team)
					{
						(++count) %= 2;
						if (count == nth)
						{
							return i;
						}
					}
				}
			}
			else
			{
				return C_PLAYER_NONE;
			}
		}
		else
		{
			return (index + 1) % 2;
		}
		

		
	}

	bool Session::AreTeamsValid() const
	{
		// We only care for teams when we have more than 2 players.
		if (mPlayers.size() > 2)
		{
			for (unsigned int i = 0; i < mPlayers.size(); ++i)
			{
				if (GetTeamMate(i) == C_PLAYER_NONE)
					return false;
			}
		}

		return true;
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
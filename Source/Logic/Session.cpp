#include "Session.hpp"
#include "Globals.hpp"
#include <cassert>

namespace Logic
{
	Session::Session(unsigned int playerCount)
		: mChatReceiver(NULL)
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

	const Grid& Session::GetGrid() const
	{
		return mGrid;
	}

	void Session::SetChatReceiver(ChatReceiver* receiver)
	{
		mChatReceiver = receiver;
	}

	ChatReceiver* Session::GetChatReceiver()
	{
		return mChatReceiver;
	}
}
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

	void Session::SetChatReceiver(ChatReceiver* receiver)
	{
		mChatReceiver = receiver;
	}

	ChatReceiver* Session::GetChatReceiver()
	{
		return mChatReceiver;
	}
}
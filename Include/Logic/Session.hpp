#ifndef SESSION_HPP
#define SESSION_HPP

#include <vector>
#include "Player.hpp"

namespace Logic
{
	class ChatReceiver
	{
	public:
		virtual void ReceiveChatMessage(const std::string& message, unsigned int sourceID) = 0;
	};

	class Session
	{
	public:
		Session(unsigned int playerCount);
		virtual ~Session() throw();

		void SetChatReceiver(ChatReceiver* receiver);
		ChatReceiver* GetChatReceiver();
	protected:
		ChatReceiver* mChatReceiver;
		std::vector<Player*> mPlayers;
	};
}

#endif
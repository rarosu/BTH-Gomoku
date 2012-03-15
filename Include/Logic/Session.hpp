#ifndef SESSION_HPP
#define SESSION_HPP

#include <vector>
#include "Player.hpp"
#include "Grid.hpp"

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

		virtual void Update(const GameTime& gameTime) = 0;

		/**
			Send a chat message to the specified target player

			If broadcast or team is used, targetID is ignored.
		*/
		virtual void SendChatMessage(const std::string& message, int targetID, Network::Recipient::Recipient recipient) = 0;
	protected:
		ChatReceiver* mChatReceiver;
		Grid mGrid;
		std::vector<Player*> mPlayers;
	};
}

#endif
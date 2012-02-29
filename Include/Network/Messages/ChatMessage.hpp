#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	namespace Recipient
	{
		enum Recipient { Broadcast, Team, Private };
	}

	class ChatMessage : public Message
	{
	public:
		ChatMessage();
		ChatMessage(int sourceID, int targetID, Recipient::Recipient recipient, const std::string& message);
		static Message* Inflate(const std::string& args);

		int mSourceID;
		int mTargetID;
		Recipient::Recipient mRecipient;
		std::string mMessage;		
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
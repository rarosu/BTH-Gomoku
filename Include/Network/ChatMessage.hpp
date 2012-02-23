#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class ChatMessage : public Message
	{
	public:
		ChatMessage();
		ChatMessage(const std::string& args);
		std::string mMessage;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
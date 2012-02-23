#include "ChatMessage.hpp"

namespace Network
{
	ChatMessage::ChatMessage()
	{

	}

	ChatMessage::ChatMessage(const std::string& args):
	mMessage(args)
	{
	}

	int ChatMessage::ID() const
	{
		return C_MESSAGE_CHAT;
	}

	std::string ChatMessage::FlattenArguments() const
	{
		return mMessage;
	}
}
#include "ChatMessage.hpp"
#include <sstream>

namespace Network
{
	ChatMessage::ChatMessage()
	{}

	ChatMessage::ChatMessage(int sourceID, int targetID, Recipient::Recipient recipient, const std::string& message):
		mSourceID(sourceID), mTargetID(targetID), mRecipient(recipient), mMessage(message)
	{}

	int ChatMessage::ID() const
	{
		return C_MESSAGE_CHAT;
	}

	std::string ChatMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mSourceID << " " << mMessage << " " << static_cast<int>(mRecipient) << " " << mTargetID;

		return s.str();
	}
}
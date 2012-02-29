#include "ChatMessage.hpp"
#include <sstream>

namespace Network
{
	ChatMessage::ChatMessage()
		: mSourceID(-1), mTargetID(-1), mRecipient(Recipient::Broadcast)
	{}

	ChatMessage::ChatMessage(int sourceID, int targetID, Recipient::Recipient recipient, const std::string& message)
		: mSourceID(sourceID), mTargetID(targetID), mRecipient(recipient), mMessage(message)
	{}

	Message* ChatMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int sourceID;
		int targetID;
		int recipient;

		s >> sourceID;
		s >> targetID;
		s >> recipient;

		return new ChatMessage(sourceID, targetID, static_cast<Recipient::Recipient>(recipient), s.str());
	}

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
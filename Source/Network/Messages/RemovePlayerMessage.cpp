#include "RemovePlayerMessage.hpp"
#include <sstream>

namespace Network
{
	RemovePlayerMessage::RemovePlayerMessage() 
		: mPlayerID(-1), mReason(Reason::TimeOut)
	{}

	RemovePlayerMessage::RemovePlayerMessage(int playerID, Reason::Reason reason) 
		: mPlayerID(playerID), mReason(reason)
	{}

	Message* RemovePlayerMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;
		int reason;

		s >> playerID;
		s >> reason;

		return new RemovePlayerMessage(playerID, static_cast<Reason::Reason>(reason));
	}

	int RemovePlayerMessage::ID() const
	{
		return C_MESSAGE_REMOVE_PLAYER;
	}

	std::string RemovePlayerMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID << " " << static_cast<int>(mReason);

		return s.str();
	}
}
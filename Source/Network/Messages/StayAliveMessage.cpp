#include "StayAliveMessage.hpp"
#include <sstream>

namespace Network
{
	StayAliveMessage::StayAliveMessage()
		: mPlayerID(-1)
	{}

	StayAliveMessage::StayAliveMessage(int playerID)
		: mPlayerID(playerID)
	{}

	Message* StayAliveMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;

		s >> playerID;

		return new StayAliveMessage(playerID);
	}

	int StayAliveMessage::ID() const
	{
		return C_MESSAGE_STAY_ALIVE;
	}

	std::string StayAliveMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID;

		return s.str();
	}
}
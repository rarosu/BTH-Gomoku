#include "LeaveGameMessage.hpp"
#include <sstream>

namespace Network
{
	LeaveGameMessage::LeaveGameMessage()
		: mPlayerID(-1)
	{}

	LeaveGameMessage::LeaveGameMessage(int playerID)
		: mPlayerID(playerID)
	{}

	Message* LeaveGameMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;

		s >> playerID;

		return new LeaveGameMessage(playerID);
	}

	int LeaveGameMessage::ID() const
	{
		return C_MESSAGE_LEAVE_GAME;
	}

	std::string LeaveGameMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID;

		return s.str();
	}
}
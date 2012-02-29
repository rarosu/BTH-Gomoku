#include "TurnMessage.hpp"
#include <sstream>

namespace Network
{
	TurnMessage::TurnMessage()
		: mPlayerID(-1)
	{}

	TurnMessage::TurnMessage(int playerID)
		: mPlayerID(playerID)
	{}

	Message* TurnMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;

		s >> playerID;

		return new TurnMessage(playerID);
	}

	int TurnMessage::ID() const
	{
		return C_MESSAGE_TURN;
	}

	std::string TurnMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID;

		return s.str();
	}
}
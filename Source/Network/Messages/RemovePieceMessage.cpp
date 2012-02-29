#include "RemovePieceMessage.hpp"
#include <sstream>

namespace Network
{
	RemovePieceMessage::RemovePieceMessage()
		: mPlayerID(-1), mX(0), mY(0)
	{}

	RemovePieceMessage::RemovePieceMessage(int playerID, int x, int y)
		: mPlayerID(playerID), mX(x), mY(y)
	{}

	Message* RemovePieceMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;
		int x;
		int y;

		s >> playerID;
		s >> x;
		s >> y;

		return new RemovePieceMessage(playerID, x, y);
	}

	int RemovePieceMessage::ID() const
	{
		return C_MESSAGE_REMOVE_PIECE;
	}

	std::string RemovePieceMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID << " " << mX << " " << mY;

		return s.str();
	}
}
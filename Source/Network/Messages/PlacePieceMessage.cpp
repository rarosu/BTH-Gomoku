#include "PlacePieceMessage.hpp"
#include <sstream>

namespace Network
{
	PlacePieceMessage::PlacePieceMessage()
		: mPlayerID(-1), mX(0), mY(0), mLifetime(-1)
	{}

	PlacePieceMessage::PlacePieceMessage(int playerID, int x, int y, int lifetime)
		: mPlayerID(playerID), mX(x), mY(y), mLifetime(lifetime)
	{}

	Message* PlacePieceMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;
		int x;
		int y;
		int lifetime;

		s >> playerID;
		s >> x;
		s >> y;
		s >> lifetime;

		return new PlacePieceMessage(playerID, x, y, lifetime);
	}

	int PlacePieceMessage::ID() const
	{
		return C_MESSAGE_PLACE_PIECE;
	}

	std::string PlacePieceMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID << " " << mX << " " << mY << " " << mLifetime;

		return s.str();
	}
}
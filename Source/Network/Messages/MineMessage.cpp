#include "MineMessage.hpp"
#include <sstream>

namespace Network
{
	MineMessage::MineMessage()
		: mX(0), mY(0) {}

	MineMessage::MineMessage(int x, int y)
		: mX(x), mY(y) {}
	
	Message* MineMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int x;
		int y;

		s >> x;
		s >> y;

		return new MineMessage(x, y);
	}

	int MineMessage::ID() const
	{
		return C_MESSAGE_HINT;
	}

	std::string MineMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mX << " " << mY;

		return s.str();
	}
}
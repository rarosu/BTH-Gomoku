#include "WarningMessage.hpp"
#include <sstream>

namespace Network
{
	WarningMessage::WarningMessage()
		: mX(0), mY(0) {}

	WarningMessage::WarningMessage(int x, int y)
		: mX(x), mY(y) {}
	
	Message* WarningMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int x;
		int y;

		s >> x;
		s >> y;

		return new WarningMessage(x, y);
	}

	int WarningMessage::ID() const
	{
		return C_MESSAGE_HINT;
	}

	std::string WarningMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mX << " " << mY;

		return s.str();
	}
}
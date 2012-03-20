#include "GeneralMessage.hpp"
#include <sstream>

namespace Network
{
	GeneralMessage::GeneralMessage()
		: mX(0), mY(0) {}

	GeneralMessage::GeneralMessage(int x, int y)
		: mX(x), mY(y) {}
	
	Message* GeneralMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int x;
		int y;

		s >> x;
		s >> y;

		return new GeneralMessage(x, y);
	}

	int GeneralMessage::ID() const
	{
		return C_MESSAGE_HINT;
	}

	std::string GeneralMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mX << " " << mY;

		return s.str();
	}
}
#include "HintMessage.hpp"
#include <sstream>

namespace Network
{
	HintMessage::HintMessage()
		: mX(0), mY(0) {}

	HintMessage::HintMessage(int x, int y)
		: mX(x), mY(y) {}
	
	Message* HintMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int x;
		int y;

		s >> x;
		s >> y;

		return new HintMessage(x, y);
	}

	int HintMessage::ID() const
	{
		return C_MESSAGE_HINT;
	}

	std::string HintMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mX << " " << mY;

		return s.str();
	}
}
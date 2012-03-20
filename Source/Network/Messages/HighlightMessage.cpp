#include "HighlightMessage.hpp"
#include <sstream>

namespace Network
{
	HighlightMessage::HighlightMessage()
		: mX(0), mY(0), mType(0) {}

	HighlightMessage::HighlightMessage(int x, int y, int type)
		: mX(x), mY(y), mType(type) {}
	
	Message* HighlightMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int x;
		int y;
		int type;

		s >> x;
		s >> y;
		s >> type;

		return new HighlightMessage(x, y, type);
	}

	int HighlightMessage::ID() const
	{
		return C_MESSAGE_HIGHLIGHT;
	}

	std::string HighlightMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mX << " " << mY << " " << mType;

		return s.str();
	}
}
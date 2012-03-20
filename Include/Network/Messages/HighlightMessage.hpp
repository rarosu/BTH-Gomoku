#ifndef HIGHLIGHT_MESSAGE_HPP
#define HIGHLIGHT_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class HighlightMessage : public Message
	{
	public:
		HighlightMessage();
		HighlightMessage(int x, int y, int type);
		static Message* Inflate(const std::string& args);

		int mX;
		int mY;
		int mType;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}


#endif
#ifndef WARNING_MESSAGE_HPP
#define WARNING_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class WarningMessage : public Message
	{
		public:
		WarningMessage();
		WarningMessage(int x, int y);
		static Message* Inflate(const std::string& args);

		int mX;
		int mY;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
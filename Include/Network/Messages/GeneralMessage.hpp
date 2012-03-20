#ifndef GENERAL_MESSAGE_HPP
#define GENERAL_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class GeneralMessage : public Message
	{
		public:
		GeneralMessage();
		GeneralMessage(int x, int y);
		static Message* Inflate(const std::string& args);

		int mX;
		int mY;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
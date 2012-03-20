#ifndef HINT_MESSAGE_HPP
#define HINT_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class HintMessage : public Message
	{
		public:
		HintMessage();
		HintMessage(int x, int y);
		static Message* Inflate(const std::string& args);

		int mX;
		int mY;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}


#endif
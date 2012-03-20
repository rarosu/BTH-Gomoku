#ifndef MINE_MESSAGE_HPP
#define MINE_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class MineMessage : public Message
	{
		public:
		MineMessage();
		MineMessage(int x, int y);
		static Message* Inflate(const std::string& args);

		int mX;
		int mY;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
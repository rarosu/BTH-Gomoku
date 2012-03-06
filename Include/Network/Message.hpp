#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

namespace Network
{
	const int C_MESSAGE_CHAT = 1;
	const int C_MESSAGE_JOIN = 2;
	const int C_MESSAGE_ACCEPT = 3;
	const int C_MESSAGE_ADD_PLAYER = 4;
	const int C_MESSAGE_REMOVE_PLAYER = 5;
	const int C_MESSAGE_SET_TEAM = 6;
	const int C_MESSAGE_SET_MARKER = 7;
	const int C_MESSAGE_PLACE_PIECE = 8;
	const int C_MESSAGE_REMOVE_PIECE = 9;
	const int C_MESSAGE_TURN = 10;
	const int C_MESSAGE_START_GAME = 11;
	const int C_MESSAGE_LEAVE_GAME = 12;
	const int C_MESSAGE_STAY_ALIVE = 13;

	class Message
	{
	public:
		std::string Flatten() const;

		virtual int ID() const = 0;
	protected:
		virtual std::string FlattenArguments() const = 0;
	};

	class MessageFactory
	{
	public:
		static Message* Inflate(const std::string& message);
	private:
	};
}

#endif
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>

namespace Network
{
	const int C_MESSAGE_CHAT = 1;
	const int C_MESSAGE_ACCEPT = 2;
	const int C_MESSAGE_ADD_PLAYER = 3;
	const int C_MESSAGE_REMOVE_PLAYER = 4;
	const int C_MESSAGE_SET_TEAM = 5;
	const int C_MESSAGE_SET_MARKER = 6;
	const int C_MESSAGE_PLACE_PIECE = 7;
	const int C_MESSAGE_REMOVE_PIECE = 8;
	const int C_MESSAGE_TURN = 9;
	const int C_MESSAGE_START_GAME = 10;
	const int C_MESSAGE_LEAVE_GAME = 11;
	const int C_MESSAGE_STAY_ALIVE = 12;

	class Message
	{
	public:
		std::string Flatten() const;
	protected:
		virtual std::string FlattenArguments() const = 0;
		virtual int ID() const = 0;
	private:
	};

	class MessageFactory
	{
	public:
		static Message* Inflate(const std::string& message);
	private:
	};
}

#endif
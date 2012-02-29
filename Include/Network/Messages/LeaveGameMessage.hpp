#ifndef LEAVE_GAME_MESSAGE_HPP
#define LEAVE_GAME_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class LeaveGameMessage : public Message
	{
	public:
		LeaveGameMessage();
		LeaveGameMessage(int playerID);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
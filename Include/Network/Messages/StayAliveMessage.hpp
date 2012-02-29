#ifndef STAY_ALIVE_MESSAGE_HPP
#define STAY_ALIVE_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class StayAliveMessage : public Message
	{
	public:
		StayAliveMessage();
		StayAliveMessage(int playerID);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
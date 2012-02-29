#ifndef TURN_MESSAGE_HPP
#define TURN_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class TurnMessage : public Message
	{
	public:
		TurnMessage();
		TurnMessage(int playerID);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}


#endif
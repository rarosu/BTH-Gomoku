#ifndef REMOVE_PLAYER_MESSAGE_HPP
#define REMOVE_PLAYER_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	namespace Reason
	{
		enum Reason { TimeOut, Left, Boot };
	}

	class RemovePlayerMessage : public Message
	{
	public:
		RemovePlayerMessage();
		RemovePlayerMessage(int playerID, Reason::Reason reason);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
		Reason::Reason mReason;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
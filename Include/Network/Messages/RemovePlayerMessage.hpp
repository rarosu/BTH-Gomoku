#ifndef REMOVE_PLAYER_MESSAGE_HPP
#define REMOVE_PLAYER_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	namespace RemovePlayerReason
	{
		enum RemovePlayerReason { TimeOut, Left, Boot };
	}

	class RemovePlayerMessage : public Message
	{
	public:
		RemovePlayerMessage();
		RemovePlayerMessage(int playerID, RemovePlayerReason::RemovePlayerReason reason);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
		RemovePlayerReason::RemovePlayerReason mReason;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
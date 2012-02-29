#ifndef ACCEPT_MESSAGE_HPP
#define ACCEPT_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class AcceptMessage : public Message
	{
	public:
		AcceptMessage();
		AcceptMessage(int numberOfPlayers, int selfID);
		static Message* Inflate(const std::string& args);

		int mNumberOfPlayers;
		int mSelfID;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
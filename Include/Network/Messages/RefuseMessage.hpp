#ifndef REFUSE_MESSAGE_HPP
#define REFUSE_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	namespace RefuseReason
	{
		enum RefuseReason { TooManyPlayers, InvalidName };
	}

	class RefuseMessage : public Message
	{
	public:
		RefuseMessage();
		RefuseMessage(RefuseReason::RefuseReason reason);
		static Message* Inflate(const std::string& args);

		RefuseReason::RefuseReason mReason;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
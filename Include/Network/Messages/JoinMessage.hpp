#ifndef JOIN_MESSAGE_HPP
#define JOIN_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class JoinMessage : public Message
	{
	public:
		JoinMessage();
		JoinMessage(const std::string& name);
		static Message* Inflate(const std::string& args);

		std::string mName;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
#include "JoinMessage.hpp"
#include <sstream>

namespace Network
{
	JoinMessage::JoinMessage() 
	{}

	JoinMessage::JoinMessage(const std::string& name)
		: mName(name)
	{}

	Message* JoinMessage::Inflate(const std::string& args)
	{
		return new JoinMessage(args);
	}

	int JoinMessage::ID() const 
	{ 
		return C_MESSAGE_JOIN; 
	}
	
	std::string JoinMessage::FlattenArguments() const
	{
		return mName;
	}
}
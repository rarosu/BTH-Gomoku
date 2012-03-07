#include "RefuseMessage.hpp"
#include <sstream>

namespace Network
{
	RefuseMessage::RefuseMessage() 
		: mReason(RefuseReason::TooManyPlayers)
	{}
	RefuseMessage::RefuseMessage(RefuseReason::RefuseReason reason) 
		: mReason(reason)
	{}

	Message* RefuseMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int reason;
		s >> reason;

		return new RefuseMessage(static_cast<RefuseReason::RefuseReason>(reason));
	}

	int RefuseMessage::ID() const 
	{ 
		return C_MESSAGE_REFUSE; 
	}
	
	std::string RefuseMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mReason;

		return s.str();
	}
}


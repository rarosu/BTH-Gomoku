#include "AcceptMessage.hpp"
#include <sstream>

namespace Network
{
	AcceptMessage::AcceptMessage() : 
		mNumberOfPlayers(0), mSelfID(0) 
	{}
	AcceptMessage::AcceptMessage(int numberOfPlayers, int selfID) :
		mNumberOfPlayers(numberOfPlayers), mSelfID(selfID) 
	{}

	int AcceptMessage::ID() const 
	{ 
		return C_MESSAGE_ACCEPT; 
	}
	
	std::string AcceptMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mNumberOfPlayers << " " << mSelfID;

		return s.str();
	}
}


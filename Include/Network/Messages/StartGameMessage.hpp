#ifndef START_GAME_MESSAGE_HPP
#define START_GAME_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class StartGameMessage : public Message
	{
	public:
		StartGameMessage();
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}


#endif
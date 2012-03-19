#ifndef GAME_OVER_MESSAGE_HPP
#define GAME_OVER_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class GameOverMessage : public Message
	{
	public:
		GameOverMessage();
		GameOverMessage(int winnerID);
		static Message* Inflate(const std::string& args);

		int mWinnerID;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}


#endif
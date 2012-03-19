#include "GameOverMessage.hpp"
#include <sstream>

namespace Network
{
	GameOverMessage::GameOverMessage()
		: mWinnerID(-1)
	{}

	GameOverMessage::GameOverMessage(int winnerID)
		: mWinnerID(winnerID)
	{}

	Message* GameOverMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int winnerID;

		s >> winnerID;

		return new GameOverMessage(winnerID);
	}

	int GameOverMessage::ID() const
	{
		return C_MESSAGE_GAME_OVER;
	}

	std::string GameOverMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mWinnerID;

		return s.str();
	}
}
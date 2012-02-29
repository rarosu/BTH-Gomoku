#include "StartGameMessage.hpp"

namespace Network
{
	StartGameMessage::StartGameMessage()
	{}

	int StartGameMessage::ID() const
	{
		return C_MESSAGE_START_GAME;
	}

	std::string StartGameMessage::FlattenArguments() const
	{
		return "";
	}
}
#include "Message.hpp"
#include "MessageInclude.hpp"
#include <sstream>

namespace Network
{


	std::string Message::Flatten() const
	{
		std::stringstream s;
		s << ID();
		s << " ";
		s << FlattenArguments();
		s << std::endl;

		return s.str();
	}



	Message* MessageFactory::Inflate(const std::string& message)
	{
		if (message == "")
			return NULL;

		// Put the message in streamform and split it by its ID and arguments
		std::stringstream s(message);
		int id;
		s >> id;
		std::string args = s.str().substr((size_t)s.tellg() + 1);
		
		// Identify the message by the ID and inflate it
		Message* m = NULL;
		switch(id)
		{
			case C_MESSAGE_CHAT:
				m = ChatMessage::Inflate(args);
				break;
			case C_MESSAGE_JOIN:
				m = JoinMessage::Inflate(args);
				break;
			case C_MESSAGE_ACCEPT:
				m = AcceptMessage::Inflate(args);
				break;
			case C_MESSAGE_ADD_PLAYER:
				m = AddPlayerMessage::Inflate(args);
				break;
			case C_MESSAGE_REMOVE_PLAYER:
				m = RemovePlayerMessage::Inflate(args);
				break;
			case C_MESSAGE_SET_TEAM:
				m = SetTeamMessage::Inflate(args);
				break;
			case C_MESSAGE_SET_MARKER:
				m = SetMarkerMessage::Inflate(args);
				break;
			case C_MESSAGE_PLACE_PIECE:
				m = PlacePieceMessage::Inflate(args);
				break;
			case C_MESSAGE_REMOVE_PIECE:
				m = RemovePieceMessage::Inflate(args);
				break;
			case C_MESSAGE_TURN:
				m = TurnMessage::Inflate(args);
				break;
			case C_MESSAGE_START_GAME:
				m = new StartGameMessage();
				break;
			case C_MESSAGE_LEAVE_GAME:
				m = LeaveGameMessage::Inflate(args);
				break;
			case C_MESSAGE_STAY_ALIVE:
				m = StayAliveMessage::Inflate(args);
				break;
		}

		return m;
	}

}
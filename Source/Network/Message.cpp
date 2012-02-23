#include "Message.hpp"
#include "ChatMessage.hpp"
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
		std::stringstream s(message);
		int id;
		s >> id;
		std::string args = s.str().substr((size_t)s.tellg() + 1);
		
		Message* m = NULL;

		switch(id)
		{
		case C_MESSAGE_CHAT:
			m = new ChatMessage(args);
			break;
		default:
			break;
		}

		return m;
	}

}
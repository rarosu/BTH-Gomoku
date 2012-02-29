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
				int sourceID;
				int targetID;
				int recipient;

				s >> sourceID;
				s >> targetID;
				s >> recipient;

				m = new ChatMessage(sourceID, targetID, static_cast<Recipient::Recipient>(recipient), s.str());
			
				break;
		}

		return m;
	}

}
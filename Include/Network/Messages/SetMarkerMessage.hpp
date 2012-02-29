#ifndef SET_MARKER_MESSAGE_HPP
#define SET_MARKER_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class SetMarkerMessage : public Message
	{
	public:
		SetMarkerMessage();
		SetMarkerMessage(int playerID, int markerID);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
		int mMarkerID;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}


#endif
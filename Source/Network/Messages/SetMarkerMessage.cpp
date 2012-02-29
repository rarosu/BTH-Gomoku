#include "SetMarkerMessage.hpp"
#include <sstream>

namespace Network
{
	SetMarkerMessage::SetMarkerMessage()
		: mPlayerID(-1), mMarkerID(-1)
	{}

	SetMarkerMessage::SetMarkerMessage(int playerID, int markerID)
		: mPlayerID(playerID), mMarkerID(markerID)
	{}

	Message* SetMarkerMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;
		int markerID;

		s >> playerID;
		s >> markerID;

		return new SetMarkerMessage(playerID, markerID);
	}

	int SetMarkerMessage::ID() const
	{
		return C_MESSAGE_SET_MARKER;
	}

	std::string SetMarkerMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID << " " << mMarkerID;

		return s.str();
	}
}
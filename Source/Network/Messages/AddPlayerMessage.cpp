#include "AddPlayerMessage.hpp"
#include <sstream>

namespace Network
{
	AddPlayerMessage::AddPlayerMessage() 
		: mPlayerID(-1), mTeam(0), mMarkerID(-1)
	{}

	AddPlayerMessage::AddPlayerMessage(int playerID, int team, int markerID, const std::string& name) 
		: mPlayerID(playerID), mTeam(team), mMarkerID(markerID), mName(name)
	{}

	Message* AddPlayerMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;
		int team;
		int markerID;

		s >> playerID;
		s >> team;
		s >> markerID;

		return new AddPlayerMessage(playerID, team, markerID, s.str());
	}

	int AddPlayerMessage::ID() const
	{
		return C_MESSAGE_ADD_PLAYER;
	}

	std::string AddPlayerMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID << " " << mTeam << " " << mMarkerID << " " << mName;

		return s.str();
	}
}
#include "SetTeamMessage.hpp"
#include <sstream>

namespace Network
{
	SetTeamMessage::SetTeamMessage() 
		: mPlayerID(-1), mTeam(0)
	{}

	SetTeamMessage::SetTeamMessage(int playerID, int team) 
		: mPlayerID(playerID), mTeam(team)
	{}

	Message* SetTeamMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;
		int team;

		s >> playerID;
		s >> team;

		return new SetTeamMessage(playerID, team);
	}

	int SetTeamMessage::ID() const
	{
		return C_MESSAGE_SET_TEAM;
	}

	std::string SetTeamMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID << " " << mTeam;

		return s.str();
	}
}
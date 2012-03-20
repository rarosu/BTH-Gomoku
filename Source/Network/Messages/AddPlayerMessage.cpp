#include "AddPlayerMessage.hpp"
#include <sstream>

namespace Network
{
	AddPlayerMessage::AddPlayerMessage() 
		: mPlayerID(-1), mTeam(0)
	{}

	AddPlayerMessage::AddPlayerMessage(int playerID, int team, const std::string& name) 
		: mPlayerID(playerID), mTeam(team), mName(name)
	{}

	Message* AddPlayerMessage::Inflate(const std::string& args)
	{
		std::stringstream s;
		s.str(args);

		int playerID;
		int team;
		std::string name;

		s >> playerID;
		s >> team;
		s.ignore();
		std::getline(s, name);

		return new AddPlayerMessage(playerID, team, name);
	}

	int AddPlayerMessage::ID() const
	{
		return C_MESSAGE_ADD_PLAYER;
	}

	std::string AddPlayerMessage::FlattenArguments() const
	{
		std::stringstream s;
		s << mPlayerID << " " << mTeam << " " << mName;

		return s.str();
	}
}
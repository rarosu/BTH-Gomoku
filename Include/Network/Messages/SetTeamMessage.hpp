#ifndef SET_TEAM_MESSAGE_HPP
#define SET_TEAM_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class SetTeamMessage : public Message
	{
	public:
		SetTeamMessage();
		SetTeamMessage(int playerID, int team);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
		int mTeam;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}


#endif
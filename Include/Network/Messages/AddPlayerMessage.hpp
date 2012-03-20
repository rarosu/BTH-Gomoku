#ifndef ADD_PLAYER_MESSAGE_HPP
#define ADD_PLAYER_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class AddPlayerMessage : public Message
	{
	public:
		AddPlayerMessage();
		AddPlayerMessage(int playerID, int team, const std::string& name);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
		int mTeam;
		std::string mName;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
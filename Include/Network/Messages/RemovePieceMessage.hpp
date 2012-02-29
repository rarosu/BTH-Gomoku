#ifndef REMOVE_PIECE_MESSAGE_HPP
#define REMOVE_PIECE_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class RemovePieceMessage : public Message
	{
	public:
		RemovePieceMessage();
		RemovePieceMessage(int playerID, int x, int y);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
		int mX;
		int mY;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}


#endif
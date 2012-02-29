#ifndef PLACE_PIECE_MESSAGE_HPP
#define PLACE_PIECE_MESSAGE_HPP

#include "Message.hpp"

namespace Network
{
	class PlacePieceMessage : public Message
	{
	public:
		PlacePieceMessage();
		PlacePieceMessage(int playerID, int x, int y, int lifetime);
		static Message* Inflate(const std::string& args);

		int mPlayerID;
		int mX;
		int mY;
		int mLifetime;
	protected:
		int ID() const;
		std::string FlattenArguments() const;
	};
}

#endif
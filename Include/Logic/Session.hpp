#ifndef SESSION_HPP
#define SESSION_HPP

#include <vector>
#include "GameTime.hpp"
#include "Player.hpp"
#include "Grid.hpp"
#include "ChatMessage.hpp"

namespace Logic
{
	/**
		A chat receiver, for getting chat messages sent to it
	*/
	class ChatReceiver
	{
	public:
		virtual void ReceiveChatMessage(const std::string& message, unsigned int sourceID) = 0;
	};



	/**
		A session holding the state of one game
	*/
	class Session
	{
	public:
		Session(unsigned int playerCount);
		virtual ~Session() throw();

		unsigned int GetPlayerCount() const;
		unsigned int GetSlotCount() const;
		unsigned int GetCurrentPlayer() const;
		std::string GetPlayerName(unsigned int index) const;

		/**
			Check whether it is a local player's turn
		*/
		virtual bool IsLocalPlayerTurn() const = 0;

		/**
			Set/Get the current chat receiver (which will have chat messages
			sent over network echoed to it)
		*/
		void SetChatReceiver(ChatReceiver* receiver);
		ChatReceiver* GetChatReceiver();

		/**
			Get the current state of the grid
		*/
		const Grid& GetGrid() const;

		/**
			Update the logic in this session
		*/
		virtual void Update(const GameTime& gameTime) = 0;

		/**
			Send a chat message to the specified target player

			If broadcast or team is used, targetID is ignored.
		*/
		virtual void SendChatMessage(const std::string& message, int targetID, Network::Recipient::Recipient recipient) = 0;

		/**
			Place a piece
		*/
		virtual void SendPlacePieceMessage(const Logic::Cell& cell) = 0;
	protected:
		ChatReceiver* mChatReceiver;
		Grid mGrid;
		std::vector<Player*> mPlayers;
		unsigned int mCurrentPlayer;
	};
}

#endif
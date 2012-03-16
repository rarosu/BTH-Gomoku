#ifndef CLIENTSESSION_HPP
#define CLIENTSESSION_HPP

#include <vector>
#include <string>
#include "GameTime.hpp"
#include "Session.hpp"
#include "Client.hpp"
#include "Player.hpp"
#include "ChatMessage.hpp"

namespace Logic
{
	class ClientNotificationInterface
	{
	public:
		virtual void GameStarted() = 0;
	};

	class ClientSession : public Session
	{
	public:
		ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID);
		~ClientSession();

		bool IsLocalPlayerTurn() const;

		/**
			Set the listener for client events
		*/
		void SetClientNotifiee(ClientNotificationInterface* notifiee);
		const ClientNotificationInterface* GetNotifiee() const;

		/**
			Update the client session (listen to server, handle messages, send stay alive messages)
		*/
		void Update(const GameTime& gameTime);

		/**
			Send a chat message to the specified target player

			If broadcast or team is used, targetID is ignored.
		*/
		void SendChatMessage(const std::string& message, int targetID, Network::Recipient::Recipient recipient);

		void SendPlacePieceMessage(const Logic::Cell& cell);
	private:
		static const float C_KEEP_ALIVE_DELAY;

		ClientNotificationInterface* mNotifiee;

		Network::Client* mClient;
		unsigned int mSelfID;
		float mKeepAliveCounter;

		bool mClientTurn;
	};
}
#endif
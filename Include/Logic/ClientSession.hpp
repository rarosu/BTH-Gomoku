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
	/**
		An interface for client events
	*/
	class ClientNotificationInterface
	{
	public:
		/**
			Sent when the server has started the game.
		*/
		virtual void GameStarted() = 0;
	};



	/**
		Every client that connects to a remote game should create a client session. This should
		be done after an Accept message has been received from the server.
	*/
	class ClientSession : public Session
	{
	public:
		ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID);
		~ClientSession();

		/**
			Returns true if the current player is self ID
		*/
		bool IsLocalPlayerTurn() const;

		/**
			Check whether a specific player is local or not.
		*/
		bool IsLocalPlayer(PlayerID index) const;

		/**
			Returns true if we're still connected to a server.
		*/
		bool IsConnected() const;

		/**
			Return the ID of the local player
		*/
		unsigned int GetSelfID() const;

		/**
			Get the type of a slot
		*/
		ClientSlot GetSlotType(PlayerID playerID) const;

		/**
			Set the listener for client events
		*/
		void SetClientNotifiee(ClientNotificationInterface* notifiee);
		const ClientNotificationInterface* GetClientNotifiee() const;

		/**
			Update the client session (listen to server, handle messages, send stay alive messages)
		*/
		void Update(const GameTime& gameTime);

		/**
			Send a chat message to the specified target player

			If broadcast or team is used, targetID is ignored.
		*/
		void SendChatMessage(const std::string& message, PlayerID targetID, Network::Recipient::Recipient recipient);

		/**
			Check if the move is valid, then send a request to put a marker in said cell to the
			server. If the move is okay, the server will send back a place piece message.
		*/
		void SendPlacePieceMessage(const Logic::Cell& cell);

		/**
			Change team, if the player is local player.
		*/
		void SendSetTeamMessage(PlayerID player, Team team);
	private:
		static const float C_KEEP_ALIVE_DELAY;

		ClientNotificationInterface* mClientNotifiee;

		Network::Client* mClient;
		unsigned int mSelfID;
		float mKeepAliveCounter;

		bool mClientTurn;
	};
}
#endif
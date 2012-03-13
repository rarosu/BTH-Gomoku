#ifndef CLIENTSESSION_HPP
#define CLIENTSESSION_HPP

#include <vector>
#include <string>
#include "Session.hpp"
#include "Client.hpp"
#include "Player.hpp"
#include "ChatMessage.hpp"

namespace Logic
{
	class ClientSession : public Session
	{
	public:
		ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID);
		~ClientSession();

		void Update();
		void SendChatMessage(const std::string& message, int targetID, Network::Recipient::Recipient recipient);

		unsigned int GetPlayerCount() const;
		std::string GetPlayerName(Network::Slot slot) const;
	private:
		Network::Client* mClient;
		unsigned int mSelfID;
	};
}
#endif
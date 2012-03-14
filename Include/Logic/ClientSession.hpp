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
	class ClientSession : public Session
	{
	public:
		ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID);
		~ClientSession();

		void Update(const GameTime& gameTime);
		void SendChatMessage(const std::string& message, int targetID, Network::Recipient::Recipient recipient);

		unsigned int GetPlayerCount() const;
		std::string GetPlayerName(Network::Slot slot) const;
	private:
		static const float C_KEEP_ALIVE_DELAY;

		Network::Client* mClient;
		unsigned int mSelfID;
		float mKeepAliveCounter;
	};
}
#endif
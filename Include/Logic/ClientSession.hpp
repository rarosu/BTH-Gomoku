#ifndef CLIENTSESSION_HPP
#define CLIENTSESSION_HPP

#include <vector>
#include <string>
#include "Session.hpp"
#include "Client.hpp"
#include "Player.hpp"

namespace Logic
{
	class ClientSession : public Session
	{
	public:
		ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID);
		~ClientSession();

		void Update();

		unsigned int GetPlayerCount() const;
		const std::string& GetPlayerName(Network::Slot slot) const;
	private:
		Network::Client* mClient;
	};
}
#endif
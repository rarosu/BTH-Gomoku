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
		ClientSession(const std::string& IPAddress);	// TODO, add port and playername
		~ClientSession();
	private:
		Network::Client* mClient;
	};



	/**
		This class will manage the joining process of a client.
		It will send a join message to the server and receive the
		Accept message. 
		
		When Accept is received, a ClientSession will be created.
	*/
	/*
	class ClientJoinSetup
	{
	public:
		ClientJoinSetup(const ClientParameters& parameters);

		ClientSession* CheckAccept();
	private:
		Network::Client mClient;
		std::string mPlayerName;

		std::vector<Player> mPlayers;
	};
	*/
}
#endif
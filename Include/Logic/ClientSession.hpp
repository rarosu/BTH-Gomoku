#ifndef CLIENTSESSION_HPP
#define CLIENTSESSION_HPP

#include "Session.hpp"
#include "Client.hpp"
#include <string>

namespace Logic
{

	class ClientSession
	{
	public:
		ClientSession(const std::string& IPAddress, unsigned short port = 6666);
		~ClientSession();
	private:
		Network::Client mClient;
	};

}
#endif
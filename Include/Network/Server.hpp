#ifndef SERVER_HPP
#define SERVER_HPP

#include "ListenSocket.hpp"
#include "ComSocket.hpp"
#include "Message.hpp"
#include <vector>

namespace Network
{
	class Server
	{
	public:
		Server(int maxClients, unsigned short port = 6666);
		~Server();
		int GetPort() const;
		void Update();
		void Send(const Message& message);
		Message* PopMessage();
	private:
		int mPort;
		int mMaxClients;
		ListenSocket mListenSocket;
		std::vector<ComSocket> mClients;
		std::vector<std::string> mMessageQueue;
	};
}

#endif
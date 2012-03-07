#ifndef SERVER_HPP
#define SERVER_HPP

#include "ListenSocket.hpp"
#include "ComSocket.hpp"
#include "Message.hpp"
#include <vector>

namespace Network
{
	class ServerEventInterface
	{
	public:
		virtual void ClientConnected(int slot) = 0;
		virtual void ClientDisconnected(int slot) = 0;
	};

	class Server
	{
	public:
		Server(int maxClients, unsigned short port = 6666);
		~Server();

		void SetEventInterface(ServerEventInterface* e);
		const ServerEventInterface* GetEventInterface() const;

		int GetPort() const;
		void Update();

		void Send(const Message& message);
		Message* PopMessage();

		void DisconnectClient(int slot);
	private:
		ServerEventInterface* mEventInterface;

		int mPort;
		int mMaxClients;
		ListenSocket mListenSocket;
		std::vector<ComSocket> mClients;
		std::vector<std::string> mMessageQueue;
	};
}

#endif
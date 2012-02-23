#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include <WinSock2.h>
#include <WS2tcpip.h>

namespace Network
{
	class ListenSocket
	{
	public:
		ListenSocket(int maxClients);
		~ListenSocket();
		int Bind(unsigned short port = 6666);
		SOCKET Accept();
		void Shutdown();
	private:
		SOCKET mSocket;
		int mMaxClients;
	};
}

#endif
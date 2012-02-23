#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include "WinInclude.hpp"


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
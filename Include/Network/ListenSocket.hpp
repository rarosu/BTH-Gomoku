#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include "WinInclude.hpp"
#include <stdexcept>


namespace Network
{
	class BindException : public std::runtime_error
	{
	public:
		BindException(int port, int errorCode);

		int GetPort() const { return mPort; }
		int GetErrorCode() const { return mErrorCode; }
	private:
		int mPort;
		int mErrorCode;
	};


	class ListenSocket
	{
	public:
		ListenSocket(int maxClients);
		~ListenSocket();
		void Bind(unsigned short port = 6666);
		SOCKET Accept();
		void Shutdown();
	private:
		SOCKET mSocket;
		int mMaxClients;
	};
}

#endif
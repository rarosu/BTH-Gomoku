#ifndef LISTENSOCKET_HPP
#define LISTENSOCKET_HPP

#include "WinInclude.hpp"
#include "NetworkInclude.hpp"
#include <stdexcept>


namespace Network
{
	class BindException : public std::runtime_error
	{
	public:
		BindException(int port, int errorCode);

		Port GetPort() const { return mPort; }
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

		Port GetPort() const { return mPort; }
		bool IsBound() const;

		void Bind(Port port = 6666);
		SOCKET Accept();
		void Shutdown();
	private:
		Port mPort;
		SOCKET mSocket;
		int mMaxClients;
	};
}

#endif
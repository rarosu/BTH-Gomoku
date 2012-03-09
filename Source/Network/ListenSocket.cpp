#include "ListenSocket.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace Network
{
	BindException::BindException(int port, int errorCode)
		: mPort(port), mErrorCode(errorCode), std::runtime_error("")
	{}



	ListenSocket::ListenSocket(int maxClients) 
		: mSocket(INVALID_SOCKET), mMaxClients(maxClients), mPort(0)
	{

	}

	ListenSocket::~ListenSocket()
	{
		Shutdown();
	}

	void ListenSocket::Bind(Port port)
	{
		int result;
		int error = 0;

		addrinfo *addrResult = NULL, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		std::stringstream s;
		s << port;

		// Resolve the local address and port to be used by the server
		result = getaddrinfo(NULL, s.str().c_str(), &hints, &addrResult);
		if (result != 0)
		{
			throw BindException(port, result);

		}

		mSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
		if (mSocket == INVALID_SOCKET)
		{
			freeaddrinfo(addrResult);
			throw BindException(port, WSAGetLastError());
		}

		unsigned long mode = 1;
		ioctlsocket(mSocket, FIONBIO, &mode);

		//Setup the TCP listening socket
		result = bind(mSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			freeaddrinfo(addrResult);
			throw BindException(port, WSAGetLastError());
		}

		freeaddrinfo(addrResult);

		mPort = port;
	}

	SOCKET ListenSocket::Accept()
	{
		SOCKET s = INVALID_SOCKET;

		int result = listen(mSocket, mMaxClients);

		if (result == SOCKET_ERROR)
		{
			result = WSAGetLastError();

			std::stringstream ss;
			ss << "listen failed with error: " << result;
			throw std::runtime_error(ss.str());
		}

		// Accept a client socket
		s = accept(mSocket, NULL, NULL);
		if (s == INVALID_SOCKET)
		{
			result = WSAGetLastError();
			
			if (result == WSAEWOULDBLOCK)
			{
				return s;
			}
			else
			{
				std::stringstream ss;
				ss << "accept failed: " << result;
			
				throw std::runtime_error(ss.str());
			}
		}
		else
		{
			unsigned long mode = 1;
			ioctlsocket(s, FIONBIO, &mode);
		}

		return s;
	}

	void ListenSocket::Shutdown()
	{
		if (mSocket != INVALID_SOCKET)
		{
			shutdown(mSocket, SD_BOTH);
			closesocket(mSocket);

			mSocket = INVALID_SOCKET;
		}
	}
}
#include "ListenSocket.hpp"
#include <sstream>
#include <iostream>

namespace Network
{
	ListenSocket::ListenSocket(int maxClients):
		mSocket(INVALID_SOCKET), mMaxClients(maxClients)
	{

	}

	ListenSocket::~ListenSocket()
	{
		
	}

	int ListenSocket::Bind(unsigned short port)
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
			std::cerr << "getaddrinfo failed: " << result << std::endl;
		}
		mSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

		if (mSocket == INVALID_SOCKET)
		{
			error = WSAGetLastError();
			std::cerr << "Error at socket(): " << error << std::endl;
			freeaddrinfo(addrResult);
		}
		unsigned long mode = 1;
		ioctlsocket(mSocket, FIONBIO, &mode);

		//Setup the TCP listening socket
		result = bind(mSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
		if (result == SOCKET_ERROR)
		{
			error = WSAGetLastError();
			std::cerr << "bind failed with error: " << error << std::endl;
			freeaddrinfo(addrResult);
		}

		freeaddrinfo(addrResult);

		return error;
	}

	SOCKET ListenSocket::Accept()
	{
		SOCKET s = INVALID_SOCKET;

		int result = listen(mSocket, mMaxClients);

		if (result == SOCKET_ERROR)
		{
			result = WSAGetLastError();
			std::cerr << "Listen failed with error: " << result << std::endl;
		}

		// Accept a client socket
		s = accept(mSocket, NULL, NULL);
		if (s == INVALID_SOCKET)
		{
			result = WSAGetLastError();
			//std::cerr << "accept failed: " << result << std::endl;
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
		}
	}
}
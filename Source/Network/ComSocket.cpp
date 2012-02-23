#include "ComSocket.hpp"

#include <sstream>
#include <iostream>

namespace Network
{

	ComSocket::ComSocket():
	mSocket(INVALID_SOCKET)
	{

	}

	ComSocket::ComSocket(SOCKET socket):
	mSocket(socket)
	{

	}

	ComSocket::~ComSocket()
	{
	}

	int ComSocket::Connect(const char* ipAddress, unsigned short port)
	{
		addrinfo *addrResult = NULL, hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		std::stringstream s;
		s << port;

		// Resolve the server and address port
		int result = getaddrinfo(ipAddress, s.str().c_str(), &hints, &addrResult);
		if (result != 0)
		{
			std::cerr << "getaddrinfo failed: " << result << std::endl;
		}

		for (addrinfo* ptr = addrResult; ptr != NULL; ptr = ptr->ai_next)
		{
			mSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
			if (mSocket == INVALID_SOCKET)
			{
				std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
			}

			// Connect to server
			result = connect(mSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (result == SOCKET_ERROR)
			{
				closesocket(mSocket);
				mSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		unsigned long mode = 1;
		ioctlsocket(mSocket, FIONBIO, &mode);

		freeaddrinfo(addrResult);

		if (mSocket == INVALID_SOCKET)
		{
			std::cerr << "Unable to connect to server!" << std::endl;
		}

		return result;
	}

	// Adds a message to the send queue, returns error or zero
	void ComSocket::Send(const std::string& message)
	{
		mMessagesToSend.push_back(message); 
	}

	// Concatenates a message to the receive buffer, returns error or zero
	int ComSocket::Receive()
	{
		int len = 0;
		int error = 0;
		char buf[1024];

		do {
			len = recv(mSocket, buf, sizeof(buf), 0);

			if (len > 0)
			{
				std::string msg(buf, len);
				mReceiveBuffer += msg;
			}
			else if (len < 0)
			{
				error = WSAGetLastError();
			}
		} while (len > 0);

		return error;
	}

	// Tries to receive data from the socket, interpret it, and send any message in the send queue
	// Returns the amount of received messages
	int ComSocket::Update()
	{
		int result = Receive();
		if (result != 0 && result != WSAEWOULDBLOCK)
			std::cerr << "Error receiving: "<< result << std::endl;

		while (mReceiveBuffer.size() > 1)
			InterpretBuffer();

		if (!mMessagesToSend.empty())
		{
			std::string m = mMessagesToSend.front();
			result = SendMessage(m);
			if (result != 0 && result != WSAEWOULDBLOCK)
				std::cerr << "Error sending: "<< result << std::endl;
			mMessagesToSend.erase(mMessagesToSend.begin());
		}

		return mReceivedMessages.size();
	}

	// Returns first message in the receive queue, or NULL
	std::string ComSocket::PopMessage()
	{
		std::string m;
		if (!mReceivedMessages.empty())
		{
			m = mReceivedMessages.front();
			mReceivedMessages.erase(mReceivedMessages.begin());
		}

		return m;
	}

	// Shuts down a socket if it's active
	void ComSocket::Shutdown()
	{
		if (mSocket != INVALID_SOCKET)
		{
			shutdown(mSocket, SD_BOTH);
			closesocket(mSocket);
		}
	}

	// Sends a Message object, returns error or zero
	int ComSocket::SendMessage(const std::string& message)
	{
		int result = 0;
		int error = 0;
		std::string msg = message;

		do {
			result = send(mSocket, msg.c_str(), msg.size(), 0);
			if (result == SOCKET_ERROR)
			{
				error = WSAGetLastError();
				break;
			}
			else
				msg = msg.substr(result);


		} while (result > 0);

		return error;
	}

	// Interprets the buffer and adds messages to the receive queue
	void ComSocket::InterpretBuffer()
	{
		std::string s;
		
		for (int i = 0; i < mReceiveBuffer.size(); ++i)
		{
			if (mReceiveBuffer[i] == '\n')
			{
				s = mReceiveBuffer.substr(0, i);
				if (s.size() > 1)
					mReceivedMessages.push_back(s);
				if (i + 1 < mReceiveBuffer.size())
					mReceiveBuffer = mReceiveBuffer.substr(i + 1);
				else
					mReceiveBuffer = "";
				break;
			}
		}
	}
}
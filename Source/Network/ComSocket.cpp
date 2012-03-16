#include "ComSocket.hpp"
#include <sstream>
#include <cassert>

namespace Network
{
	ConnectionFailure::ConnectionFailure(const std::string& errorMessage)
		: std::runtime_error(errorMessage)
	{}

	ComSocket::ComSocket()
		: mSocket(INVALID_SOCKET), mConnected(false)
	{
		
	}

	ComSocket::ComSocket(SOCKET socket)
		: mSocket(socket), mConnected(true)
	{
		int optVal;
		int optLen = sizeof(int);
		int error = getsockopt(mSocket, SOL_SOCKET, SO_ACCEPTCONN, (char*)&optVal, &optLen);
		if (error == SOCKET_ERROR)
		{
			std::stringstream ss;
			ss << "Socket error: " << error;

			throw std::runtime_error(ss.str());
		}
	}

	ComSocket::~ComSocket()
	{
		Shutdown();
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
			std::stringstream ss;
			ss << "getaddrinfo failed: " << result;

			throw ConnectionFailure(ss.str());
		}

		for (addrinfo* ptr = addrResult; ptr != NULL; ptr = ptr->ai_next)
		{
			mSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
			if (mSocket == INVALID_SOCKET)
			{
				std::stringstream ss;
				ss << "socket failed with error: " << WSAGetLastError();

				throw ConnectionFailure(ss.str());
			}

			// Connect to server
			result = connect(mSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (result == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error == WSAECONNREFUSED)
				{
					throw ConnectionFailure("Connection refused.");
				}
				closesocket(mSocket);
				
				mSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		if (mSocket == INVALID_SOCKET)
		{
			std::stringstream ss;
			ss << "Unable to connect to server with error: " << WSAGetLastError();

			throw ConnectionFailure(ss.str());
		}

		unsigned long mode = 1;
		ioctlsocket(mSocket, FIONBIO, &mode);

		freeaddrinfo(addrResult);

		mConnected = true;

		return result;
	}

	// Adds a message to the send queue, returns error or zero
	void ComSocket::Send(const std::string& message)
	{
		mMessagesToSend.push_back(message); 
	}

	// Concatenates a message to the receive buffer, returns true if connected
	bool ComSocket::Receive()
	{
		int len = 0;
		int error = 0;
		char buf[1024];
		ZeroMemory(buf, 1024);

		do {
			len = recv(mSocket, buf, sizeof(buf), 0);

			if (len > 0)
			{
				std::string msg(buf, len);
				mReceiveBuffer += msg;
			}
			else if (len == 0)
				mConnected = false;
			else
			{
				error = WSAGetLastError();
				if (error != WSAEWOULDBLOCK)
				{
					if (error == WSAECONNRESET)
					{
						mConnected = false;
						break;
					}

					std::stringstream s;
					s << "recv failed: " << error;

					if (error == WSAECONNABORTED)
					{
						throw ConnectionFailure("Connection aborted");
					}

					throw std::runtime_error(s.str());
				}
			}
		} while (len > 0);

		return mConnected;
	}

	// Tries to receive data from the socket, interpret it, and send any message in the send queue
	// Returns the amount of received messages
	int ComSocket::Update()
	{
		Receive();

		while (mReceiveBuffer.size() > 1)
			InterpretBuffer();

		if (!mMessagesToSend.empty())
		{
			std::string m = mMessagesToSend.front();
			SendMessage(m);
			
			mMessagesToSend.erase(mMessagesToSend.begin());
		}

		return mReceivedMessages.size();
	}

	// Returns first message in the receive queue, or NULL
	std::string ComSocket::PopMessage()
	{
		return PopMessage(GetQueuedMessageCount() - 1);
	}

	std::string ComSocket::PopMessage(unsigned int index)
	{
		if (mReceivedMessages.empty())
			return "";

		assert(index >= 0);
		assert(index < mReceivedMessages.size());

		std::string m;
		if (!mReceivedMessages.empty())
		{
			m = mReceivedMessages[index];
			mReceivedMessages.erase(mReceivedMessages.begin() + index);
		}

		return m;
	}

	std::string ComSocket::PeekMessage(unsigned int index) const
	{
		if (mReceivedMessages.empty())
			return "";

		assert(index >= 0);
		assert(index < mReceivedMessages.size());

		std::string m;
		if (!mReceivedMessages.empty())
		{
			m = mReceivedMessages[index];
		}

		return m;
	}

	unsigned int ComSocket::GetQueuedMessageCount() const
	{
		return mReceivedMessages.size();
	}

	// Shuts down a socket if it's active
	void ComSocket::Shutdown()
	{
		if (mSocket != INVALID_SOCKET)
		{
			shutdown(mSocket, SD_BOTH);
			closesocket(mSocket);

			mSocket = INVALID_SOCKET;
		}
	}

	// Sends a Message object, returns error or zero
	void ComSocket::SendMessage(const std::string& message)
	{
		int result = 0;
		std::string msg = message;

		do {
			result = send(mSocket, msg.c_str(), msg.size(), 0);
			if (result == SOCKET_ERROR)
			{
				int error = WSAGetLastError();

				if (error != WSAEWOULDBLOCK)
				{
					std::stringstream s;
					s << "send failed: " << error;
					
					switch (error)
					{
					case WSAECONNRESET:
						throw ConnectionFailure("Connection reset by peer");
					case WSAECONNABORTED:
						throw ConnectionFailure("Connection aborted. Check your firewall options.");
					default:
						throw std::runtime_error(s.str());
					}
				}
				break;
			}
			else
				msg = msg.substr(result);


		} while (result > 0);
	}

	// Interprets the buffer and adds messages to the receive queue
	void ComSocket::InterpretBuffer()
	{
		std::string s;
		
		for (unsigned int i = 0; i < mReceiveBuffer.size(); ++i)
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

	bool ComSocket::IsConnected() const
	{
		return mConnected;
	}
}
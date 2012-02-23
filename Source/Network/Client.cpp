#include "Client.hpp"
#include <iostream>

namespace Network
{
	Client::Client(const char* ipAddress, unsigned short port)
	{
		WSADATA wsaData;

		int result;
		result = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (result != 0)
		{
			std::cerr << "WSAStartup failed: " << result << std::endl;
		}

		mSocket.Connect(ipAddress, port);
	}

	Client::~Client()
	{
		mSocket.Shutdown();

		WSACleanup();
	}

	void Client::Send(const Message& message)
	{
		mSocket.Send(message.Flatten());
	}

	Message* Client::PopMessage()
	{
		return MessageFactory::Inflate(mSocket.PopMessage());
	}

	int Client::Update()
	{
		return mSocket.Update();
	}
}
#include "Server.hpp"
#include <iostream>

namespace Network
{
	Server::Server(int maxClients, unsigned short port):
	mPort(port), mMaxClients(maxClients), mListenSocket(maxClients)
	{
		WSADATA wsaData;

		int result;
		result = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (result != 0)
		{
			std::cerr << "WSAStartup failed: " << result << std::endl;
		}

		mListenSocket.Bind(port);
	}

	Server::~Server()
	{
		mListenSocket.Shutdown();
		for (int i = 0; i < mClients.size(); ++i)
			mClients[i].Shutdown();

		WSACleanup();

	}

	int Server::GetPort() const
	{
		return mPort;
	}

	void Server::Update()
	{
		if (mClients.size() < mMaxClients)
		{
			SOCKET s = INVALID_SOCKET;
			s = mListenSocket.Accept();

			if (s != INVALID_SOCKET)
				mClients.push_back(ComSocket(s));
		}

		

	
		for (int i = 0; i < mClients.size(); ++i)
		{
			mClients[i].Update();
			std::string m;
			while ((m = mClients[i].PopMessage()) != "")
				mMessageQueue.push_back(m);
		}
		
	}

	void Server::Send(const Message& message)
	{
		if (!mClients.empty())
		{
			for (int i = 0; i < mClients.size(); ++i)
				mClients[i].Send(message.Flatten());
		}
	}

	Message* Server::PopMessage()
	{
		Message* m = NULL;
		if (!mMessageQueue.empty())
		{
			m = MessageFactory::Inflate(mMessageQueue.front());
			mMessageQueue.erase(mMessageQueue.begin());
		}

		return m;
	}
}
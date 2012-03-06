#include "Server.hpp"
#include <iostream>

namespace Network
{
	Server::Server(ServerEventInterface* eventInterface, int maxClients, unsigned short port)
		: mEventInterface(eventInterface)
		, mPort(port)
		, mMaxClients(maxClients)
		, mListenSocket(maxClients)
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
		for (unsigned int i = 0; i < mClients.size(); ++i)
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
			{
				// Add client and notify connect
				mClients.push_back(ComSocket(s));
				mEventInterface->ClientConnected(mClients.size() - 1);
			}
		}
	
		for (unsigned int i = 0; i < mClients.size(); ++i)
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
			for (unsigned int i = 0; i < mClients.size(); ++i)
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
#include "Server.hpp"
#include "Globals.hpp"
#include <sstream>
#include <cassert>

namespace Network
{
	SlotMessage::SlotMessage()
		: mMessage(NULL), mSlot(0) {}

	SlotMessage::SlotMessage(Message* message, Slot slot)
		: mMessage(message), mSlot(slot) {}

	SlotMessage::~SlotMessage()
	{
		SafeDelete(mMessage);
	}

	Server::Server(int maxClients, unsigned short port)
		: mEventInterface(NULL)
		, mPort(port)
		, mMaxClients(maxClients)
		, mListenSocket(maxClients)
	{
		WSADATA wsaData;

		int result;
		result = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (result == SOCKET_ERROR)
		{
			std::stringstream ss;
			ss << "WSAStartup failed: " << WSAGetLastError();

			throw std::runtime_error(ss.str());
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

	void Server::SetEventInterface(ServerEventInterface* e)
	{
		mEventInterface = e;
	}

	const ServerEventInterface* Server::GetEventInterface() const
	{
		return mEventInterface;
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

				if (mEventInterface != NULL)
					mEventInterface->ClientConnected(mClients.size() - 1);
			}
		}
	
		for (Slot i = 0; i < mClients.size(); ++i)
		{
			if (mClients[i].IsConnected())
			{
				mClients[i].Update();
				std::string m;
				while ((m = mClients[i].PopMessage()) != "")
					mMessageQueue.push_back(m);
			}
			else
			{
				mClients.erase(mClients.begin() + i);
				--i;

				if (mEventInterface != NULL)
					mEventInterface->ClientDisconnected(i);
			}
		}
		
	}

	void Server::Send(const Message& message)
	{
		for (Slot i = 0; i < mClients.size(); ++i)
		{
			Send(i, message);
		}
	}

	void Server::Send(Slot slot, const Message& message)
	{
		assert(slot >= 0);
		assert(slot < mClients.size());

		mClients[slot].Send(message.Flatten());
	}

	SlotMessage Server::PopMessage()
	{
		/*
		SlotMessage m;
		if (!mMessageQueue.empty())
		{
			m.mMessage = MessageFactory::Inflate(mMessageQueue.front().mMessage);
			mMessageQueue.erase(mMessageQueue.begin());
		}

		return m;
		*/
	}

	void Server::DisconnectClient(Slot slot)
	{
		assert(slot >= 0);
		assert(slot < mClients.size());
		
		mClients[slot].Shutdown();
		if (mEventInterface != NULL)
			mEventInterface->ClientDisconnected(slot);
	}
}
#include "Server.hpp"
#include "Globals.hpp"
#include <sstream>
#include <cassert>

namespace Network
{
	SlotMessage::SlotMessage()
		: mMessage(NULL), mSlot(-1) {}

	SlotMessage::SlotMessage(Message* message, Slot slot)
		: mMessage(message), mSlot(slot) {}


	Server::Server(int maxClients, Port port)
		: mEventInterface(NULL)
		, mListenSocket(maxClients)
	{
		// Resize the client vector
		mClients.resize(maxClients);
		for (unsigned int i = 0; i < mClients.size(); ++i)
		{
			mClients[i] = NULL;
		}

		// Start up WinSock
		WSADATA wsaData;

		int result;
		result = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (result != 0)
		{
			std::stringstream ss;
			ss << "WSAStartup failed: " << result;

			throw std::runtime_error(ss.str());
		}

		// Bind to the given port
		mListenSocket.Bind(port);
	}

	Server::~Server()
	{
		// Stop listening and shutdown all clients
		mListenSocket.Shutdown();
		for (unsigned int i = 0; i < mClients.size(); ++i)
		{
			if (mClients[i] != NULL)
			{
				mClients[i]->Shutdown();
				SafeDelete(mClients[i]);
			}
		}

		// Shutdown WinSock
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

	Port Server::GetPort() const
	{
		return mListenSocket.GetPort();
	}

	void Server::Update()
	{
		// Find a free slot in the client list
		int slot = -1;
		if ( (slot = GetFreeSlot()) != -1 )
		{
			SOCKET s = INVALID_SOCKET;
			s = mListenSocket.Accept();

			if (s != INVALID_SOCKET)
			{
				// Add client and notify connect
				mClients[slot] = new ComSocket(s);

				if (mEventInterface != NULL)
					mEventInterface->ClientConnected(slot);
			}
		}

		// Update the clients
		for (Slot i = 0; i < mClients.size(); ++i)
		{
			if (mClients[i] != NULL)
			{
				if (mClients[i]->IsConnected())
				{
					mClients[i]->Update();
					std::string m;
					while ((m = mClients[i]->PopMessage()) != "")
						mMessageQueue.push_back(SlotMessage(MessageFactory::Inflate(m), i));
				}
				else
				{
					DisconnectClient(i);
				}
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

		mClients[slot]->Send(message.Flatten());
	}

	SlotMessage Server::PopMessage()
	{
		SlotMessage m;

		if (!mMessageQueue.empty())
		{
			m = mMessageQueue.front();
			mMessageQueue.erase(mMessageQueue.begin());
		}

		return m;
	}

	void Server::DisconnectClient(Slot slot)
	{
		assert(slot >= 0);
		assert(slot < mClients.size());
		assert(mClients[slot] != NULL);
		
		mClients[slot]->Shutdown();
		SafeDelete(mClients[slot]);

		if (mEventInterface != NULL)
			mEventInterface->ClientDisconnected(slot);
	}

	int Server::GetFreeSlot() const
	{
		for (unsigned int i = 0; i < mClients.size(); ++i)
		{
			if (mClients[i] == NULL)
				return i;
		}

		return -1;
	}
}
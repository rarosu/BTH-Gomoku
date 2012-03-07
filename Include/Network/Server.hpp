#ifndef SERVER_HPP
#define SERVER_HPP

#include "ListenSocket.hpp"
#include "ComSocket.hpp"
#include "Message.hpp"
#include <vector>

namespace Network
{
	typedef unsigned int Slot;
	typedef unsigned short Port;

	class ServerEventInterface
	{
	public:
		virtual void ClientConnected(Slot slot) = 0;
		virtual void ClientDisconnected(Slot slot) = 0;
	};

	struct SlotMessage
	{
	public:
		SlotMessage();
		SlotMessage(Message* message, Slot slot);
		~SlotMessage();

		Message* mMessage;
		Slot mSlot;
	};

	class Server
	{
	public:
		Server(int maxClients, Port port = 6666);
		~Server();

		void SetEventInterface(ServerEventInterface* e);
		const ServerEventInterface* GetEventInterface() const;

		Port GetPort() const;
		void Update();

		void Send(const Message& message);
		void Send(Slot slot, const Message& message);
		SlotMessage PopMessage();

		void DisconnectClient(Slot slot);
	private:
		ServerEventInterface* mEventInterface;

		Port mPort;
		unsigned int mMaxClients;
		ListenSocket mListenSocket;
		std::vector<ComSocket> mClients;
		std::vector<SlotMessage> mMessageQueue;
	};
}

#endif
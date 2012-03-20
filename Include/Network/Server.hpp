#ifndef SERVER_HPP
#define SERVER_HPP

#include "NetworkInclude.hpp"
#include "ListenSocket.hpp"
#include "ComSocket.hpp"
#include "Message.hpp"
#include <vector>

namespace Network
{
	/**
		An interface for notification on server events
	*/
	class ServerEventInterface
	{
	public:
		virtual void ClientConnected(Slot slot) = 0;
		virtual void ClientDisconnected(Slot slot) = 0;
	};

	/**
		Owns a message that needs to be manually removed
	*/
	struct SlotMessage
	{
	public:
		SlotMessage();
		SlotMessage(Message* message, Slot slot);

		Message* mMessage;
		Slot mSlot;
	};

	/**
		A network server, which listens for incoming connections
		and holds a number of client sockets that it can communicate
		with.
	*/
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
		bool IsConnected(Slot slot) const;
		void ShutdownListenSocket();
	private:
		ServerEventInterface* mEventInterface;

		ListenSocket mListenSocket;
		std::vector<ComSocket*> mClients;
		std::vector<SlotMessage> mMessageQueue;

		int GetFreeSlot() const;
	};
}

#endif
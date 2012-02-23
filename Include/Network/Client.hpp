#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ComSocket.hpp"
#include "Message.hpp"

namespace Network
{
	class Client
	{
	public:
		Client(const char* ipAddress = "127.0.0.1", unsigned short port = 6666);
		~Client();
		void Send(const Message& message);
		Message* PopMessage();
		int Update();
	private:
		ComSocket mSocket;

	};
}

#endif
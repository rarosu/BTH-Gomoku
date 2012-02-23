#ifndef COMSOCKET_HPP
#define COMSOCKET_HPP

#include "WinInclude.hpp"
#include <vector>
#include "Message.hpp"

namespace Network
{
	class ComSocket
	{
	public:
		ComSocket();
		ComSocket(SOCKET socket);
		~ComSocket();
		int Connect(const char* ipAddress = "127.0.0.1", unsigned short port = 6666);
		void Send(const std::string& message);
		int Receive();
		int Update();
		std::string PopMessage();
		void Shutdown();
	private:
		int SendMessage(const std::string& message);
		void InterpretBuffer();

		SOCKET mSocket;
		std::vector<std::string> mMessagesToSend;
		std::vector<std::string> mReceivedMessages;
		std::string mReceiveBuffer;
	};
}

#endif
#ifndef COMSOCKET_HPP
#define COMSOCKET_HPP

#include <vector>
#include <stdexcept>
#include <fstream>
#include "WinInclude.hpp"
#include "Message.hpp"


namespace Network
{
	class ConnectionFailure : public std::runtime_error
	{
	public:
		ConnectionFailure(const std::string& errorMessage);
	};

	class ComSocket
	{
	public:
		ComSocket();
		ComSocket(SOCKET socket);
		~ComSocket();

		int Connect(const char* ipAddress = "127.0.0.1", unsigned short port = 6666);
		void Send(const std::string& message);
		bool Receive();

		int Update();
		std::string PopMessage();
		std::string PopMessage(unsigned int index);
		std::string PeekMessage(unsigned int index) const;
		unsigned int GetQueuedMessageCount() const;

		void Shutdown();
		bool IsConnected() const;
	private:
		void SendMessage(const std::string& message);
		void InterpretBuffer();

		bool mConnected;
		SOCKET mSocket;
		std::vector<std::string> mMessagesToSend;
		std::vector<std::string> mReceivedMessages;
		std::string mReceiveBuffer;
	};
}

#endif
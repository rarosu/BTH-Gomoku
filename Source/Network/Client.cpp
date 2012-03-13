#include "Client.hpp"
#include <stdexcept>
#include <sstream>

namespace Network
{
	Client::Client(const char* ipAddress, Port port)
	{
		WSADATA wsaData;

		int result;
		result = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (result != 0)
		{
			std::stringstream ss;
			ss << "WSAStartup failed: ";
			ss << result;
			throw std::runtime_error(ss.str());
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

	Message* Client::PopMessage(unsigned int index)
	{
		return MessageFactory::Inflate(mSocket.PopMessage(index));
	}

	int Client::PeekMessageID(unsigned int index) const
	{
		Message* m = MessageFactory::Inflate(mSocket.PeekMessage(index));
		return m->ID();
	}

	unsigned int Client::GetQueuedMessageCount() const
	{
		return mSocket.GetQueuedMessageCount();
	}

	int Client::Update()
	{
		return mSocket.Update();
	}
}
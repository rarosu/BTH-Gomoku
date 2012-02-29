#include "ClientSession.hpp"

namespace Logic
{
	ClientSession::ClientSession(const std::string& IPAddress, unsigned short port):
		mClient(IPAddress.c_str(), port)
	{
		
	}

	ClientSession::~ClientSession()
	{
		
	}
}
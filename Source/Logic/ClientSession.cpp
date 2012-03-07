#include "ClientSession.hpp"

namespace Logic
{
	ClientSession::ClientSession(const std::string& IPAddress)
		: mClient(NULL)
	{
		
		//mClient.Send(Network::JoinMessage(parameters.PlayerName));
	}

	ClientSession::~ClientSession()
	{
		SafeDelete(mClient);
	}
}
#include "ClientSession.hpp"

namespace Logic
{
	ClientSession::ClientSession(const ClientParameters& parameters)
		: mClient(parameters.mIPAddress.c_str(), parameters.mPort)
	{
		//mClient.Send(Network::JoinMessage(parameters.PlayerName));
	}

	ClientSession::~ClientSession()
	{
		
	}
}
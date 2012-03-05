#include "ServerSession.hpp"

namespace Logic
{
	ServerSession::ServerSession(const ServerParameters& parameters) 
		: mServer(parameters.mRuleset->GetPlayerCount(), parameters.mPort)
		, mRuleset(parameters.mRuleset)
	{
		mPlayers.resize(mRuleset->GetPlayerCount());
		
		// TODO: Set admin to local controller
		mPlayers[0].SetController(NULL);
		mPlayers[0].SetName(parameters.mAdminName);
	}

	ServerSession::~ServerSession() throw()
	{

	}
}

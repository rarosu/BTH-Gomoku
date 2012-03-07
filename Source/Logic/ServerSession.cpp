#include "ServerSession.hpp"

namespace Logic
{
	ServerSession::ServerSession(const ServerParameters& parameters) 
		: mServer(parameters.mRuleset->GetPlayerCount(), parameters.mPort)
		, mRuleset(parameters.mRuleset)
	{
		mServer.SetEventInterface(this);
		mPlayers.resize(mRuleset->GetPlayerCount());
		
		// TODO: Set admin to local controller
		mPlayers[0].SetController(NULL);
		mPlayers[0].SetName(parameters.mAdminName);
	}

	ServerSession::~ServerSession() throw()
	{
		SafeDelete(mRuleset);
	}

	unsigned short ServerSession::GetPort() const
	{
		return mServer.GetPort();
	}

	const Ruleset* ServerSession::GetRuleset() const
	{
		return mRuleset;
	}

	void ServerSession::ClientConnected(int slot)
	{

	}

	void ServerSession::ClientDisconnected(int slot)
	{

	}
}

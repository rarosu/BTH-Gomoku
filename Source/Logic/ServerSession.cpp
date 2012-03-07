#include "ServerSession.hpp"

namespace Logic
{
	ServerSession::ServerSession(Network::Server* server, const std::string& adminName, Ruleset* ruleset) 
		: mServer(server)
		, mRuleset(ruleset)
	{
		mServer->SetEventInterface(this);
		mPlayers.resize(mRuleset->GetPlayerCount());
		
		// TODO: Set admin to local controller
		mPlayers[0].SetController(NULL);
		mPlayers[0].SetName(adminName);
	}

	ServerSession::~ServerSession() throw()
	{
		SafeDelete(mServer);
		SafeDelete(mRuleset);
	}

	unsigned short ServerSession::GetPort() const
	{
		return mServer->GetPort();
	}

	const Ruleset* ServerSession::GetRuleset() const
	{
		return mRuleset;
	}

	void ServerSession::ClientConnected(int slot)
	{
		if (slot >= mPlayers.size())
		{
			// No room for the player
			mServer->DisconnectClient(slot);
		}
		else
		{

		}
	}

	void ServerSession::ClientDisconnected(int slot)
	{

	}
}

#include "ServerGameState.hpp"

namespace State
{
	ServerGameState::ServerGameState(StateID id, ID3D10Device* device)
		: AbstractGameState(id, device)
		, mServerSession(NULL)
	{

	}

	ServerGameState::~ServerGameState() throw()
	{
		
	}

	void ServerGameState::SetServerSession(Logic::ServerSession* serverSession)
	{
		mServerSession = serverSession;
		SetSession(mServerSession);
	}
}
#include "ClientGameState.hpp"

namespace State
{
	ClientGameState::ClientGameState(StateID id, ID3D10Device* device)
		: AbstractGameState(id, device)
		, mClientSession(NULL)
	{}

	ClientGameState::~ClientGameState() throw()
	{
		
	}

	void ClientGameState::SetClientSession(Logic::ClientSession* clientSession)
	{
		mClientSession = clientSession;
		SetSession(mClientSession);
	}

	void ClientGameState::InitializeGame()
	{
		SetChatName(mClientSession->GetPlayerName(mClientSession->GetSelfID()));
	}

	void ClientGameState::EndGame()
	{

	}
}

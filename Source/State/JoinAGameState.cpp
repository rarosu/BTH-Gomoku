#include "JoinAGameState.hpp"

namespace State
{
	JoinAGameState::JoinAGameState(StateID id, ID3D10Device* device, LobbyState* lobbyState)
		: ApplicationState(id),
		mDevice(device),
		mLo
	{

	}

	JoinAGameState::~JoinAGameState() throw()
	{

	}


	void JoinAGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{

	}

	void JoinAGameState::Draw()
	{

	}


	void JoinAGameState::OnStatePushed()
	{

	}

	void JoinAGameState::OnStatePopped()
	{

	}

}
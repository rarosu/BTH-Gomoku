#ifndef SERVER_GAME_STATE_HPP
#define SERVER_GAME_STATE_HPP

#include "AbstractGameState.hpp"
#include "Grid.hpp"
#include "Scene.hpp"
#include "Marker.hpp"
#include "ComponentGroup.hpp"
#include "ServerSession.hpp"

namespace State
{
	class ServerGameState : public AbstractGameState
	{
	public:
		ServerGameState(StateID id, ID3D10Device* device);
		~ServerGameState() throw();

		void SetServerSession(Logic::ServerSession* serverSession);
	private:
		ID3D10Device* mDevice;

		Logic::ServerSession* mServerSession;
	};
}

#endif
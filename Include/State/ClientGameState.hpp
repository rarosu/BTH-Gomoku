#ifndef CLIENT_GAME_STATE_HPP
#define CLIENT_GAME_STATE_HPP

#include "AbstractGameState.hpp"
#include "Grid.hpp"
#include "Scene.hpp"
#include "Marker.hpp"
#include "ComponentGroup.hpp"
#include "ClientSession.hpp"

namespace State
{
	class ClientGameState : public AbstractGameState
	{
	public:
		ClientGameState(StateID id, ID3D10Device* device);
		~ClientGameState() throw();

		void SetClientSession(Logic::ClientSession* clientSession);
	protected:
		void OnConnectionFailure();
		void InitializeGame();
		void EndGame();

		bool CanSendChatMessage() const;
	private:
		ID3D10Device* mDevice;

		Logic::ClientSession* mClientSession;
	};
}

#endif
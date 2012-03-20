#ifndef CLIENT_LOBBY_STATE_HPP
#define CLIENT_LOBBY_STATE_HPP

#include "AbstractLobbyState.hpp"
#include "ClientSession.hpp"
#include "ClientGameState.hpp"

namespace State
{
	class ClientLobbyState : public AbstractLobbyState, public Logic::ClientNotificationInterface
	{
	public:
		ClientLobbyState(StateID id, ID3D10Device* device, State::ClientGameState* clientGameState);

		void SetSessionArguments(Network::Client* client, int playerCount, int selfID, const std::string& name);

		void GameStarted();
	protected:
		void AppendStatePushed();
		void AppendStatePopped();
		void AppendComponents();
	private:
		Logic::ClientSession*					mClientSession;
		State::ClientGameState*					mClientGameState;

		void CreateComponents();
	};
}
#endif
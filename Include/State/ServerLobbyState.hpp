#ifndef SERVER_LOBBY_STATE_HPP
#define SERVER_LOBBY_STATE_HPP

#include "AbstractLobbyState.hpp"
#include "ServerSession.hpp"
#include "ServerGameState.hpp"

namespace State
{
	class ServerLobbyState : public AbstractLobbyState
	{
	public:
		ServerLobbyState(StateID id, ID3D10Device* device, State::ServerGameState* serverGameState);

		void SetSessionArguments(Network::Server* server, const std::string& adminName, Logic::Ruleset* ruleset);
	protected:
		void AppendStatePushed();
		void AppendStatePopped();
		void AppendComponents();
		void AppendUpdate();
	private:
		Components::TextButton*					mStartButton;

		Logic::ServerSession*					mServerSession;
		State::ServerGameState*					mServerGameState;
	};
}
#endif
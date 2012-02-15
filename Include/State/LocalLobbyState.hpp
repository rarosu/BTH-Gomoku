#ifndef LOBBY_STATE_HPP
#define LOBBY_STATE_HPP

#include "ApplicationState.hpp"

namespace State
{
	class LocalLobbyState : public ApplicationState
	{
	public:
		LocalLobbyState(StateID id);

		void Update(const InputManager& input, const GameTime& gameTime);
		void Draw();
	private:

	};
}

#endif
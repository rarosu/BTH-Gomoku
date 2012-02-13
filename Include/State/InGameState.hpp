#ifndef MAIN_STATE_HPP
#define MAIN_STATE_HPP

#include "ApplicationState.hpp"

namespace State
{
	class InGameState : public ApplicationState
	{
	public:
		InGameState(StateID id);

		void Update(const InputManager& input, const GameTime& gameTime);
		void Draw();
	private:

	};
}

#endif
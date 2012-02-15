#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "ApplicationState.hpp"

namespace State
{
	class MenuState : public ApplicationState
	{
	public:
		MenuState(StateID id);

		void Update(const InputManager& input, const GameTime& gameTime);
		void Draw();
	private:

	};
}

#endif
#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "GameTime.hpp"
#include "InputManager.hpp"

namespace Components
{
	class Component
	{
	public:
		Component();

		virtual void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState) = 0;
		virtual void Draw() = 0;

	protected:
		RECT					mPositionRect;

		float GetWidth() const;
		float GetHeight() const;	
	};
}
#endif
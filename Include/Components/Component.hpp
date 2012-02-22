#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "GameTime.hpp"
#include "InputManager.hpp"
#include "Viewport.hpp"

class Game;

namespace Components
{
	class Component
	{
	public:
		friend class Game;

		Component();
		~Component() throw();

		virtual void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState) = 0;
		virtual void Draw() = 0;
		virtual void LostFocus() = 0;
		virtual void GotFocus() = 0;

	protected:
		RECT						mPositionRect;
		static const Viewport*		sViewport;

		float GetWidth() const;
		float GetHeight() const;
		void SetFocusThis();
		void LoseFocus();

	private:
		static Component*			sHasFocus;
	};
}
#endif
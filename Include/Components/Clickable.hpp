#ifndef CLICKABLE_HPP
#define CLICKABLE_HPP

#include "Component.hpp"
#include "InputManager.hpp"

namespace Components
{
	class Clickable : public Component, public MouseListener
	{
	public:
		Clickable(InputSubscription* manager);
		~Clickable() throw();

		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		virtual void Draw() = 0;
		virtual void LostFocus() = 0;
		virtual void GotFocus() = 0;

		bool IsPressed() const;
		bool IsHovered() const;

		void MouseButtonPressed(int index);
		void MouseButtonReleased(int index);
		void MouseWheelMoved(short delta);

	protected:
		//RECT			mPosition;

		virtual void MouseEntered() = 0;
		virtual void MouseExited() = 0;
		virtual void MousePressed(int buttonIndex) = 0;
		virtual void MouseReleased(int buttonIndex) = 0;

	private:
		bool				mHovered;
		bool				mPressed;

		InputSubscription*	mManager;

		Clickable(const Clickable& copy) {}
		Clickable& operator=(const Clickable& copy) {}
	};
}
#endif
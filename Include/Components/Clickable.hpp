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

		bool IsPressed() const;
		bool IsHovered() const;
		bool GetAndResetClickStatus();

		void MouseButtonPressed(int index, const InputState& currentState);
		void MouseButtonReleased(int index, const InputState& currentState);
		void MouseWheelMoved(short delta, const InputState& currentState);

	protected:
		virtual void MouseEntered() = 0;
		virtual void MouseExited() = 0;
		virtual void MousePressed(int buttonIndex) = 0;
		virtual void MouseReleased(int buttonIndex) = 0;

	private:
		bool				mHovered;
		bool				mPressed;
		bool				mIsClicked;

		InputSubscription*	mManager;

		Clickable(const Clickable& copy) {}
		Clickable& operator=(const Clickable& copy) {}
	};
}
#endif
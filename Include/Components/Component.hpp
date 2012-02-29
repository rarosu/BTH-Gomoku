#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "GameTime.hpp"
#include "InputManager.hpp"
#include "Viewport.hpp"

class Game;

namespace Components 
{
	class ComponentGroup;

	class Component : public MouseListener, public KeyListener
	{
	public:
		friend class Game;

		Component(ComponentGroup* ownerGroup);
		virtual ~Component() throw();

		virtual void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState) = 0;
		virtual void Draw() = 0;
		virtual void LostFocus() = 0;
		virtual void GotFocus() = 0;
		
		bool IsEnabled();
		void SetEnabled(bool isEnabled);
		bool IsVisible();
		void SetVisible(bool isVisible);

		// Methods inherited from MouseListener
		virtual void MouseButtonPressed(int index, const InputState& currentState) {}
		virtual void MouseButtonReleased(int index, const InputState& currentState) {}
		virtual void MouseWheelMoved(short delta, const InputState& currentState) {}

		// Methods inherited from KeyListener
		virtual void KeyPressed(int code, const InputState& currentState) {}
		virtual void KeyReleased(int code, const InputState& currentState) {}
		virtual void CharEntered(unsigned char symbol, const InputState& currentState) {}

		// DEBUG
		virtual std::string GetName() = 0;
		
	protected:
		RECT						mPositionRect;
		ComponentGroup*				mOwner;

		static const Viewport*		sViewport;

		float GetWidth() const;
		float GetHeight() const;

		void SetFocusThis();
		void LoseFocus();

	private:
		bool						mIsVisible;
		bool						mIsEnabled;
	};
}
#endif
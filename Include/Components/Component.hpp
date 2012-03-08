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

		Component(ComponentGroup* ownerGroup, RECT position);
		virtual ~Component() throw();

		virtual void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		virtual void Draw() = 0;
		virtual void LostFocus() = 0;
		virtual void GotFocus() = 0;
		
		virtual bool HasFocus() const;
		void SetFocus();
		bool IsEnabled() const;
		void SetEnabled(bool isEnabled);
		bool IsVisible() const;
		void SetVisible(bool isVisible);
		bool IsPressed() const;
		bool IsHovered() const;
		bool GetAndResetClickStatus();

		// Methods inherited from MouseListener
		virtual void MouseButtonPressed(int index, const InputState& currentState);
		virtual void MouseButtonReleased(int index, const InputState& currentState);
		virtual void MouseWheelMoved(short delta, const InputState& currentState) {}

		// Methods inherited from KeyListener
		virtual void KeyPressed(int code, const InputState& currentState) {}
		virtual void KeyReleased(int code, const InputState& currentState) {}
		virtual void CharEntered(unsigned char symbol, const InputState& currentState) {}

		// DEBUG
		virtual std::string GetName() = 0;
		
	protected:
		ComponentGroup*				mOwner;

		static const Viewport*		sViewport;

		float GetWidth() const;
		float GetHeight() const;
		const RECT& GetBoundingRect() const;
		D3DXVECTOR2 GetPosition() const;
		void LoseFocus();

		virtual void MouseEntered() = 0;
		virtual void MouseExited() = 0;
		virtual void MousePressed(int buttonIndex) = 0;
		virtual void MouseReleased(int buttonIndex) = 0;

	private:
		RECT						mPositionRect;
		bool						mIsVisible;
		bool						mIsEnabled;
		bool						mIsHovered;
		bool						mIsPressed;
		bool						mIsClicked;
	};
}
#endif
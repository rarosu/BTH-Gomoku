#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "GameTime.hpp"
#include "InputManager.hpp"
#include "Viewport.hpp"
#include "SoundManager.hpp"

class Game;

namespace Components 
{
	class ComponentGroup;

	class Component : public MouseListener, public KeyListener
	{
	public:
		friend class Game;
		friend class ComponentGroup;

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
		virtual bool IsHovered() const;
		virtual bool GetAndResetClickStatus();

		// Get the dimensions & position of the component
		float GetWidth() const;
		float GetHeight() const;
		D3DXVECTOR2 GetPosition() const;
		const RECT& GetBoundingRect() const;
		void SetPosition(D3DXVECTOR2 newPosition);

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

		// Static methods
		static void Initialize();

	protected:
		ComponentGroup*				mOwner;

		static const Viewport*		sViewport;
		static Sound*				sSndButtonClick;
		static Sound*				sSndInputFull;

		void LoseFocus();

		virtual void MouseEntered() = 0;
		virtual void MouseExited() = 0;
		virtual void MousePressed(int buttonIndex, const InputState& currentState) = 0;
		virtual void MouseReleased(int buttonIndex, const InputState& currentState) = 0;

		virtual void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState) = 0;

	private:
		RECT						mPositionRect;
		bool						mIsVisible;
		bool						mIsEnabled;
		bool						mIsHovered;
		bool						mIsPressed;
		bool						mIsClicked;

		void SetBoundingRect(const RECT& r);
	};
}
#endif
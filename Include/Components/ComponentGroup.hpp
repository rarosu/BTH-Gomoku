#ifndef COMPONENTGROUP_HPP
#define COMPONENTGROUP_HPP

#include <vector>
#include <string>

#include "Globals.hpp"
#include "Component.hpp"
#include "InputManager.hpp"

namespace Components
{
	class RootComponent;

	class ComponentGroup : public Component
	{
	public:
		friend class RootComponent;

		ComponentGroup(ComponentGroup* ownerGroup, std::string name, RECT position);
		virtual ~ComponentGroup() throw();

		void AddComponent(Component* component);
		void RemoveComponent(Component* component);
		virtual void SetFocusedComponent(Component* component);
		const Component* GetFocusedComponent();

		// Methods inherited from Component
		virtual void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		virtual void Draw();
		void LostFocus();
		void GotFocus();
		// DEBUG
		std::string GetName();

		// Methods inherited from MouseListener
		virtual void MouseButtonPressed(int index, const InputState& currentState);
		virtual void MouseButtonReleased(int index, const InputState& currentState);
		virtual void MouseWheelMoved(short delta, const InputState& currentState);

		// Methods inherited from KeyListener
		virtual void KeyPressed(int code, const InputState& currentState);
		virtual void KeyReleased(int code, const InputState& currentState);
		virtual void CharEntered(unsigned char symbol, const InputState& currentState);

		bool IsHovered() const;
		void SetBoundingRect(const RECT& r);
	protected:
		std::vector<Component*>		mComponents;

	private:
		Component*					mFocusedComponent;
		
		// DEBUG
		std::string					mName;

		ComponentGroup();

		// Methods inherited by Component
		virtual void MouseEntered() {}
		virtual void MouseExited() {}
		virtual void MousePressed(int buttonIndex, const InputState& currentState) {}
		virtual void MouseReleased(int buttonIndex, const InputState& currentState) {}
	};
}
#endif
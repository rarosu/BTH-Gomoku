#ifndef COMPONENTGROUP_HPP
#define COMPONENTGROUP_HPP

#include <vector>

#include "Component.hpp"
#include "InputManager.hpp"

class Game;

namespace Components
{
	class ComponentGroup : public Component
	{
	public:
		friend class Game;

		ComponentGroup(ComponentGroup* ownerGroup);
		virtual ~ComponentGroup() throw();

		void AddComponent(Component* component);
		void RemoveComponent(Component* component);
		void GiveFocus();
		void SetFocus(Component* component);

		// Methods inherited from Component
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		void LostFocus();
		void GotFocus();

		// Methods inherited from MouseListener
		virtual void MouseButtonPressed(int index, const InputState& currentState);
		virtual void MouseButtonReleased(int index, const InputState& currentState);
		virtual void MouseWheelMoved(short delta, const InputState& currentState);

		// Methods inherited from KeyListener
		virtual void KeyPressed(int code, const InputState& currentState);
		virtual void KeyReleased(int code, const InputState& currentState);
		virtual void CharEntered(unsigned char symbol, const InputState& currentState);

	private:
		Component*					mFocusedComponent;

		std::vector<Component*>		mComponents;

		ComponentGroup();
	};
}
#endif
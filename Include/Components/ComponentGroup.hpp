#ifndef COMPONENTGROUP_HPP
#define COMPONENTGROUP_HPP

#include <vector>

#include "Component.hpp"

namespace Components
{
	class ComponentGroup : public Component
	{
	public:
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		void AddComponent(Component* component);
		void RemoveComponent(Component* component);
		void LostFocus();
		void GotFocus();

	private:
		std::vector<Component*>		mComponents;
	};
}
#endif
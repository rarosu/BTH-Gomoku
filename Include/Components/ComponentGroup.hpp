#ifndef COMPONENTGROUP_HPP
#define COMPONENTGROUP_HPP

#include <vector>

#include "Component.hpp"

namespace Components
{
	class ComponentGroup : public Component
	{
	public:
		void AddComponent(Component* component);
		void RemoveComponent(Component* component);
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();

	private:
		std::vector<Component*>		mComponents;
	};
}
#endif
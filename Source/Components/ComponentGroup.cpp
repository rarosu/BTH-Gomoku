#include <algorithm>
#include "ComponentGroup.hpp"

namespace Components
{
	void ComponentGroup::AddComponent(Component* component)
	{
		mComponents.push_back(component);
	}

	void ComponentGroup::RemoveComponent(Component* component)
	{
		std::vector<Component*>::iterator index = std::find(mComponents.begin(), mComponents.end(), component);

		if(index != mComponents.end())
			mComponents.erase(index);
	}

	void ComponentGroup::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		std::vector<Component*>::iterator it;
		
		for(it = mComponents.begin(); it != mComponents.end(); it++)
			(*it)->Update(gameTime, currInputState, prevInputState);
	}

	void ComponentGroup::Draw()
	{
		std::vector<Component*>::iterator it;
		
		for(it = mComponents.begin(); it != mComponents.end(); it++)
			(*it)->Draw();
	}
}

#include <algorithm>
#include <cassert>
#include "ComponentGroup.hpp"

namespace Components
{
	ComponentGroup::ComponentGroup(ComponentGroup* ownerGroup, std::string name)
		: Component(ownerGroup),
		  mFocusedComponent(NULL), mName(name)
	{
		assert(ownerGroup != NULL);
	}
	
	ComponentGroup::ComponentGroup()
		: Component(NULL),
		  mFocusedComponent(NULL), mName("Root Component Group")
	{
	}

	ComponentGroup::~ComponentGroup()
	{
		for(unsigned int i = 0; i < mComponents.size(); ++i)
		{
			SafeDelete(mComponents[i]);
		}
	}

	void ComponentGroup::AddComponent(Component* component)
	{
		std::vector<Component*>::iterator index = std::find(mComponents.begin(), mComponents.end(), component);

		if(index == mComponents.end())
			mComponents.push_back(component);
	}

	void ComponentGroup::RemoveComponent(Component* component)
	{
		std::vector<Component*>::iterator index = std::find(mComponents.begin(), mComponents.end(), component);
		if(mFocusedComponent == component)
			mFocusedComponent = NULL;

		if(index != mComponents.end())
		{
			SafeDelete(*index);
			mComponents.erase(index);
		}
	}

	void ComponentGroup::SetFocus(Component* component)
	{
		if(mFocusedComponent)
			mFocusedComponent->LostFocus();

		mFocusedComponent = component;
		if(mFocusedComponent)
			mFocusedComponent->GotFocus();
	}

	void ComponentGroup::GiveFocus()
	{
		if(mOwner)
			mOwner->SetFocus(this);
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

	void ComponentGroup::LostFocus()
	{
		std::vector<Component*>::iterator it;
		
		for(it = mComponents.begin(); it != mComponents.end(); it++)
			(*it)->LostFocus();
	}

	void ComponentGroup::GotFocus()
	{
		std::vector<Component*>::iterator it;
		
		for(it = mComponents.begin(); it != mComponents.end(); it++)
			(*it)->GotFocus();
	}

	void ComponentGroup::MouseButtonPressed(int index, const InputState& currentState)
	{
		std::vector<Component*>::iterator it;
		
		for(it = mComponents.begin(); it != mComponents.end(); it++)
			(*it)->MouseButtonPressed(index, currentState);
	}

	void ComponentGroup::MouseButtonReleased(int index, const InputState& currentState)
	{
		std::vector<Component*>::iterator it;
		
		for(it = mComponents.begin(); it != mComponents.end(); it++)
			(*it)->MouseButtonReleased(index, currentState);
	}

	void ComponentGroup::MouseWheelMoved(short delta, const InputState& currentState)
	{
		std::vector<Component*>::iterator it;
		
		for(it = mComponents.begin(); it != mComponents.end(); it++)
			(*it)->MouseWheelMoved(delta, currentState);
	}

	void ComponentGroup::KeyPressed(int code, const InputState& currentState)
	{
		if(mFocusedComponent)
			mFocusedComponent->KeyPressed(code, currentState);
	}

	void ComponentGroup::KeyReleased(int code, const InputState& currentState)
	{
		if(mFocusedComponent)
			mFocusedComponent->KeyReleased(code, currentState);
	}

	void ComponentGroup::CharEntered(unsigned char symbol, const InputState& currentState)
	{
		if(mFocusedComponent)
			mFocusedComponent->CharEntered(symbol, currentState);
	}

	// DEBUG
	std::string ComponentGroup::GetName()
	{
		std::string result = mName;

		if(mFocusedComponent)
			result += " -> " + mFocusedComponent->GetName();

		return result;
	}
}

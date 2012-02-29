#include "RootComponent.hpp"
#include "ComponentGroup.hpp"
#include <cassert>

namespace Components
{
	RootComponent* RootComponent::sInstance		= NULL;

	RootComponent::RootComponent(ID3D10Device* device, int width, int height)
	{
		assert(sInstance == NULL);
		sInstance = this;

		RECT consolePos = { 0, 0, width, height / 2 };
		mConsole  = new Components::Console(device, this, consolePos, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
		AddComponent(mConsole);
	}

	RootComponent::~RootComponent()
	{
		sInstance = NULL;
	}

	void RootComponent::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		ComponentGroup::Update(gameTime, currInputState, prevInputState);

		if(currInputState.Keyboard.keyIsPressed[VK_CONTROL])
			if(currInputState.Keyboard.keyIsPressed['T'])
				if(!(prevInputState.Keyboard.keyIsPressed['T']))
				{
					mConsole->Toggle();
				}
	}

	void RootComponent::Draw()
	{
		for(int i = mComponents.size() - 1; i >= 0; --i)
		{
			mComponents[i]->Draw();
		}
	}

	void RootComponent::SetFocus(Component* component)
	{
		if(component == NULL)
		{
			if(mFocusedComponent)
				mFocusedComponent->LostFocus();
			
			if(mComponents.size() > 1)
				SetFocus(mComponents[1]);
		}
		else
			ComponentGroup::SetFocus(component);
	}

	// STATIC METHODS
	Console& RootComponent::GetConsole()
	{
		return *(sInstance->mConsole);
	}
}
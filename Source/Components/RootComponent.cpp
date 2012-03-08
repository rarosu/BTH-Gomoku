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
		mConsole->SetVisible(false);
		AddComponent(mConsole);
	}

	RootComponent::~RootComponent()
	{
		sInstance = NULL;
	}

	void RootComponent::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(currInputState.Keyboard.keyIsPressed[VK_CONTROL])
			if(currInputState.Keyboard.keyIsPressed['T'])
				if(!(prevInputState.Keyboard.keyIsPressed['T']))
				{
					mConsole->Toggle();
				}

		ComponentGroup::Refresh(gameTime, currInputState, prevInputState);
	}

	void RootComponent::Draw()
	{
		if(!IsVisible())
			return;

		for(int i = mComponents.size() - 1; i >= 0; --i)
		{
			mComponents[i]->Draw();
		}
	}

	void RootComponent::SetFocusedComponent(Component* component)
	{
		if(component == NULL)
		{
			if(mFocusedComponent)
				mFocusedComponent->LostFocus();
			
			if(mComponents.size() > 1)
				SetFocusedComponent(mComponents[1]);
		}
		else
			ComponentGroup::SetFocusedComponent(component);
	}

	bool RootComponent::HasFocus()
	{
		return true;
	}

	/*D3DXVECTOR2 RootComponent::GetPosition() const
	{
		return D3DXVECTOR2(mPositionRect.left, mPositionRect.top);
	}*/

	// STATIC METHODS
	Console& RootComponent::GetConsole()
	{
		return *(sInstance->mConsole);
	}
}
#include "Clickable.hpp"

namespace Components
{
	Clickable::Clickable(InputSubscription* manager)
		: mHovered(false), mPressed(false), mManager(manager)
	{
		mManager->AddMouseListener(this);
	}

	Clickable::~Clickable() throw()
	{
		mManager->RemoveMouseListener(this);
	}

	void Clickable::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(currInputState.Mouse.x > mPositionRect.left && currInputState.Mouse.x < mPositionRect.right &&
		   currInputState.Mouse.y > mPositionRect.top && currInputState.Mouse.y < mPositionRect.bottom)
		{
			if(!mHovered)
			{
				mHovered = true;
				MouseEntered();
			}
		}
		else
		{
			if(mHovered)
			{
				mHovered = false;
				MouseExited();
			}
		}
	}

	bool Clickable::IsPressed() const
	{
		return mPressed;
	}

	bool Clickable::IsHovered() const
	{
		return mHovered;
	}

	void Clickable::MouseButtonPressed(int index)
	{
		if(mHovered)
		{
			mPressed = true;
			MousePressed(index);
		}
	}

	void Clickable::MouseButtonReleased(int index)
	{
		if(mPressed)
		{
			mPressed = false;

			if(mHovered)
				MouseReleased(index);	
		}
	}

	void Clickable::MouseWheelMoved(short delta)
	{
	}
}
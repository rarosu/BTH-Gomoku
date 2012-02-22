#include "Clickable.hpp"

namespace Components
{
	Clickable::Clickable(InputSubscription* manager)
		: mHovered(false), mPressed(false), mManager(manager), mIsClicked(false)
	{
		//mManager->AddMouseListener(this);
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

	bool Clickable::GetAndResetClickStatus()
	{
		bool result = mIsClicked;
		mIsClicked = false;
		return result;
	}

	void Clickable::MouseButtonPressed(int index, const InputState& currentState)
	{
		if(mHovered)
		{
			mPressed = true;
			MousePressed(index);
		}
	}

	void Clickable::MouseButtonReleased(int index, const InputState& currentState)
	{
		if(mPressed)
		{
			mPressed = false;

			if(mHovered)
			{
				SetFocusThis();
				MouseReleased(index);
				mIsClicked = true;
			}
		}
	}

	void Clickable::MouseWheelMoved(short delta, const InputState& currentState)
	{
	}
}
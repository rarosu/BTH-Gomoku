#include "Clickable.hpp"

namespace Components
{
	Clickable::Clickable(ComponentGroup* ownerGroup, RECT position)
		: Component(ownerGroup, position),
		  mHovered(false), mPressed(false), mIsClicked(false)
	{
	}

	Clickable::~Clickable() throw()
	{
	}

	void Clickable::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(!IsVisible())
			return;

		D3DXVECTOR2 position = GetPosition();

		if(currInputState.Mouse.x > position.x && currInputState.Mouse.x < (position.x + GetWidth()) &&
		   currInputState.Mouse.y > position.y && currInputState.Mouse.y < (position.y + GetHeight()))
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
				SetFocus();
				MouseReleased(index);
				mIsClicked = true;
			}
		}
	}

	void Clickable::MouseWheelMoved(short delta, const InputState& currentState)
	{
	}
}
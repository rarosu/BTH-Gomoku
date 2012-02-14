#include "Clickable.hpp"

Clickable::Clickable(InputSubscription* manager)
	: mHovered(false), mPressed(false), mManager(manager)
{
	mManager->AddMouseListener(this);
	ZeroMemory(&mPosition, sizeof(mPosition));
}

Clickable::~Clickable() throw()
{
	mManager->RemoveMouseListener(this);
}

void Clickable::Update(const InputState& previousState, const InputState& currentState)
{
	if(currentState.Mouse.x > mPosition.left && currentState.Mouse.x < mPosition.right &&
	   currentState.Mouse.y > mPosition.top && currentState.Mouse.y < mPosition.bottom)
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
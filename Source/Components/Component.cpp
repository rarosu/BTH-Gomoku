#include "Component.hpp"
#include "ComponentGroup.hpp"

namespace Components
{
	const Viewport*		Component::sViewport = NULL;
	Sound*				Component::sButtonClick = NULL;

	Component::Component(ComponentGroup* ownerGroup, RECT position)
		: mOwner(ownerGroup), mPositionRect(position), mIsEnabled(true), mIsVisible(true), mIsHovered(false),
		  mIsPressed(false), mIsClicked(false)
	{		
		if(mOwner)
			mOwner->AddComponent(this);
	}

	Component::~Component() throw()
	{
	}

	void Component::Initialize()
	{
		sButtonClick = SoundManager::GetInstance().CreateSound("Resources/Sounds/buttonClick.wav");
	}

	void Component::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(!IsVisible() || !IsEnabled())
			return;

		D3DXVECTOR2 position = GetPosition();

		if(currInputState.Mouse.x > position.x && currInputState.Mouse.x < (position.x + GetWidth()) &&
		   currInputState.Mouse.y > position.y && currInputState.Mouse.y < (position.y + GetHeight()))
		{
			if(!mIsHovered)
			{
				mIsHovered = true;
				MouseEntered();
			}
		}
		else
		{
			if(mIsHovered)
			{
				mIsHovered = false;
				MouseExited();
			}
		}

		Refresh(gameTime, currInputState, prevInputState);
	}

	float Component::GetWidth() const
	{
		return std::abs((float)mPositionRect.right - mPositionRect.left);
	}

	float Component::GetHeight() const
	{
		return std::abs((float)mPositionRect.bottom - mPositionRect.top);
	}

	D3DXVECTOR2 Component::GetPosition() const
	{
		if(mOwner == NULL)
			return D3DXVECTOR2(mPositionRect.left, mPositionRect.top);
		else
			return D3DXVECTOR2(mPositionRect.left, mPositionRect.top) + mOwner->GetPosition();

	}

	void Component::SetPosition(D3DXVECTOR2 newPosition)
	{
		RECT newPosRect = { newPosition.x, newPosition.y, newPosition.x + GetWidth(), newPosition.y + GetHeight() };
		mPositionRect = newPosRect;
	}

	void Component::LoseFocus()
	{
		if(mOwner)
			mOwner->SetFocusedComponent(NULL);
	}
	
	bool Component::HasFocus() const
	{
		if (mOwner == NULL)
			return true;

		bool hasFocus = (mOwner->GetFocusedComponent() == this);
		if(mOwner != NULL)
			hasFocus = hasFocus && mOwner->HasFocus();

		return hasFocus;
	}

	void Component::SetFocus()
	{
		if(mOwner)
			mOwner->SetFocusedComponent(this);
	}

	bool Component::IsEnabled() const
	{
		return mIsEnabled;
	}

	void Component::SetEnabled(bool isEnabled)
	{
		mIsEnabled = isEnabled;
		if(mIsEnabled == false)
		{
			mIsHovered = false;
			mIsClicked = false;
		}
	}

	bool Component::IsVisible() const
	{
		return mIsVisible;
	}

	void Component::SetVisible(bool isVisible)
	{
		mIsVisible = isVisible;
	}

	bool Component::IsPressed() const
	{
		return mIsPressed;
	}

	bool Component::IsHovered() const
	{
		return mIsHovered;
	}

	bool Component::GetAndResetClickStatus()
	{
		bool result = mIsClicked;
		mIsClicked = false;
		return result;
	}

	void Component::MouseButtonPressed(int index, const InputState& currentState)
	{
		if(mIsHovered)
		{
			mIsPressed = true;
			MousePressed(index, currentState);
		}
	}

	void Component::MouseButtonReleased(int index, const InputState& currentState)
	{
		if(mIsPressed)
		{
			mIsPressed = false;

			if(mIsHovered)
			{
				SetFocus();
				MouseReleased(index, currentState);
				mIsClicked = true;
				if(sButtonClick != NULL)
					SoundManager::GetInstance().PlaySound(sButtonClick);
				else
					mIsClicked = mIsClicked;
			}
		}
	}

	const RECT& Component::GetBoundingRect() const
	{
		D3DXVECTOR2 position = GetPosition();
		RECT posRect = { position.x, position.y, position.x + GetWidth(), position.y + GetHeight() };
		return posRect;
	}
}
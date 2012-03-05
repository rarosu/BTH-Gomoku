#include "Component.hpp"
#include "ComponentGroup.hpp"

namespace Components
{
	const Viewport*		Component::sViewport = NULL;

	Component::Component(ComponentGroup* ownerGroup)
		: mIsEnabled(true), mIsVisible(true), mOwner(ownerGroup)
	{
		ZeroMemory(&mPositionRect, sizeof(mPositionRect));
		
		if(mOwner)
			mOwner->AddComponent(this);
	}

	Component::~Component() throw()
	{
	}

	float Component::GetWidth() const
	{
		return std::abs((float)mPositionRect.right - mPositionRect.left);
	}

	float Component::GetHeight() const
	{
		return std::abs((float)mPositionRect.bottom - mPositionRect.top);
	}

	void Component::SetFocusThis()
	{
		if(mOwner)
			mOwner->SetFocus(this);
	}

	void Component::LoseFocus()
	{
		if(mOwner)
			mOwner->SetFocus(NULL);
	}

	bool Component::IsEnabled()
	{
		return mIsEnabled;
	}

	void Component::SetEnabled(bool isEnabled)
	{
		mIsEnabled = isEnabled;
	}

	bool Component::IsVisible()
	{
		return mIsVisible;
	}

	void Component::SetVisible(bool isVisible)
	{
		mIsVisible = isVisible;
	}

	const RECT& Component::GetBoundingRect() const
	{
		return mPositionRect;
	}
}
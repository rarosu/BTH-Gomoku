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

	void Component::LoseFocus()
	{
		if(mOwner)
			mOwner->SetFocusedComponent(NULL);
	}
	
	bool Component::HasFocus()
	{
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
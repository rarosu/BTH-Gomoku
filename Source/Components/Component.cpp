#include "Component.hpp"

namespace Components
{
	Component* Component::mHasFocus = NULL;

	Component::Component()
	{
		ZeroMemory(&mPositionRect, sizeof(mPositionRect));
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
		if(mHasFocus)
			mHasFocus->LostFocus();

		mHasFocus = this;
		mHasFocus->GotFocus();
	}

	void Component::LoseFocus()
	{
		if(mHasFocus)
			mHasFocus->LostFocus();
		mHasFocus = NULL;
	}
}
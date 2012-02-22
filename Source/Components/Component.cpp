#include "Component.hpp"

namespace Components
{
	Component*			Component::sHasFocus = NULL;
	const Viewport*		Component::sViewport = NULL;

	Component::Component()
	{
		ZeroMemory(&mPositionRect, sizeof(mPositionRect));
	}

	Component::~Component() throw()
	{
		if(sHasFocus == this)
			sHasFocus = NULL;
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
		if(sHasFocus)
			sHasFocus->LostFocus();

		sHasFocus = this;
		sHasFocus->GotFocus();
	}

	void Component::LoseFocus()
	{
		if(sHasFocus)
			sHasFocus->LostFocus();
		sHasFocus = NULL;
	}
}
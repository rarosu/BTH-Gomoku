#include "Component.hpp"

namespace Components
{
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
}
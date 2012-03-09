#ifndef PIEMENU_HPP
#define PIEMENU_HPP

#include "Component.hpp"
#include "ComponentGroup.hpp"
#include "Sprite.hpp"

namespace Components
{
	class PieMenuItem : public Component
	{
	public:
	private:
		Sprite*				mIdleSprite;
		Sprite*				mHoverSprite;
	};

	class PieMenu : public ComponentGroup
	{
	public:

	private:
		
	};
}
#endif
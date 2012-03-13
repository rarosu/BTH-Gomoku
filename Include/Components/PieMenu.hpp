#ifndef PIEMENU_HPP
#define PIEMENU_HPP

#include "WinInclude.hpp"
#include <vector>

#include "Component.hpp"
#include "ComponentGroup.hpp"
#include "Sprite.hpp"

namespace Components
{
	class PieMenuItem : public Component
	{
	public:
		PieMenuItem(ComponentGroup* ownerGroup, RECT position, ID3D10Device* device, std::string idleSpriteFilename, 
					std::string hoveredSpriteFilename);

		bool GetAndResetClickStatus(const std::string& caption);
		void SetSelected(bool isSelected);

		// Methods inherited from Component
		void Draw();
		void LostFocus();
		void GotFocus();
		std::string GetName();			// DEBUG

	protected:
		void MouseEntered();
		void MouseExited();
		void MousePressed(int buttonIndex, const InputState& currentState);
		void MouseReleased(int buttonIndex, const InputState& currentState);

		void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);

	private:
		Sprite*						mIdleSprite;
		Sprite*						mHoverSprite;

		bool						mIsSelected;
	};

	class PieMenu : public ComponentGroup
	{
	public:
		PieMenu(ComponentGroup* ownerGroup, RECT position);

		void AddItem(std::string idleSpriteFilename, std::string hoveredSpriteFilename);

		// Methods inherited from Component
		void MousePressed(int buttonIndex, const InputState& currentState);
		void MouseReleased(int buttonIndex, const InputState& currentState);

	private:
		std::vector<PieMenuItem>	mItems;
	};
}
#endif
#ifndef PIEMENU_HPP
#define PIEMENU_HPP

#include "WinInclude.hpp"
#include <vector>

#include "Component.hpp"
#include "ComponentGroup.hpp"
#include "Sprite.hpp"
#include "GameFont.hpp"

namespace Components
{
	class PieMenuItem : public Component
	{
	public:
		PieMenuItem(ComponentGroup* ownerGroup, RECT position, ID3D10Device* device, std::string idleSpriteFilename, 
					std::string hoveredSpriteFilename);
		~PieMenuItem() throw();

		bool GetAndResetClickStatus(const std::string& caption) { return false; }
		bool IsSelected();
		void SetSelected(bool isSelected);

		// Methods inherited from Component
		void Draw();
		void LostFocus();
		void GotFocus();
		std::string GetName();			// DEBUG

		static const int			C_ITEM_SIZE;

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
		static const int			C_ITEM_NONE = -1;
		static const int			C_ITEM_NEXT = 0;
		static const int			C_ITEM_WARNING = 1;
		static const int			C_ITEM_HINT = 2;
		static const int			C_ITEM_OTHER = 3;
		static const int			C_ITEM_COUNT = 4;

		PieMenu(ComponentGroup* ownerGroup, ID3D10Device* device, RECT position);
		~PieMenu() throw();

		int GetAndResetClickedItemIndex();

		// Override methods in ComponentGroup
		virtual void MouseButtonPressed(int index, const InputState& currentState);
		virtual void MouseButtonReleased(int index, const InputState& currentState);
		virtual void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		
		// DEBUG
		void Draw();

	private:
		ID3D10Device*				mDevice;
		float						mMinDist;
		float						mMaxDist;
		int							mClickedItemIndex;

		// DEBUG
		GameFont*					mDefaultFont;
		std::string					mOutput;
	};
}
#endif
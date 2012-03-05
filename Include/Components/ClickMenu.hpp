#ifndef CLICKMENU_HPP
#define CLICKMENU_HPP

#include "GameFont.hpp"
#include "ComponentGroup.hpp"
#include "Button.hpp"

namespace Components
{
	class ClickMenu;

	class MenuItem : public Button
	{
	public:
		MenuItem(ClickMenu* ownerGroup);

		void AddSubItem(const std::string& caption);

		void Initialize(ID3D10Device* device, RECT position, std::string caption);
		void Draw();
		// DEBUG
		virtual std::string GetName();

	private:
		ClickMenu*					mSubMenu;
		GameFont*					mFont;
		std::string					mCaption;
		D3DXCOLOR					mTextColor;
	};


	class ClickMenu : public ComponentGroup
	{
	public:
		ClickMenu(ComponentGroup* ownerGroup, 
				  ID3D10Device* device, 
				  const POINT& position, 
				  int itemWidth, 
				  int itemHeight);

		void AddMenuItem(std::string caption);

	private:
		ID3D10Device*				mDevice;
		std::vector<MenuItem*>		mItems;
		int							mItemWidth;
		int							mItemHeight;
	};
}
#endif
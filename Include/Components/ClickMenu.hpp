#ifndef CLICKMENU_HPP
#define CLICKMENU_HPP

#include <map>
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
		~MenuItem() throw();

		void AddSubItem(const std::string& caption);
		ClickMenu* GetSubMenu();

		void Initialize(ID3D10Device* device, RECT position, const std::string& caption);
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

		void AddMenuItem(const std::string& caption);

		bool GetAndResetClickStatus(const std::string& caption);
		MenuItem* GetMenuItem(const std::string& caption);
		ClickMenu* GetSubMenu(const std::string& caption);
	private:
		typedef std::map<std::string, MenuItem*> ItemMap;

		ID3D10Device*						mDevice;
		ItemMap								mItems;
		int									mItemWidth;
		int									mItemHeight;
	};
}
#endif
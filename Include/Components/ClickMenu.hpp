#ifndef CLICKMENU_HPP
#define CLICKMENU_HPP

#include <map>
#include "GameFont.hpp"
#include "ComponentGroup.hpp"
#include "Button.hpp"

namespace Components
{
	class ClickMenu;

	class ClickMenuItem : public Button
	{
	public:
		ClickMenuItem(ClickMenu* ownerGroup, RECT position);
		~ClickMenuItem() throw();

		void AddSubItem(const std::string& caption);
		ClickMenu* GetSubMenu();

		void Initialize(ID3D10Device* device, const std::string& caption);
		void Draw();

		// DEBUG
		virtual std::string GetName();

	protected:
		virtual void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);

	private:
		ClickMenu*						mSubMenu;

		GameFont*						mFont;
		std::string						mCaption;
		D3DXCOLOR						mTextColor;
	};

	class ClickMenu : public ComponentGroup
	{
	public:
		ClickMenu(ComponentGroup* ownerGroup, 
				  ID3D10Device* device, 
				  RECT position, 
				  int itemWidth, 
				  int itemHeight);

		void AddMenuItem(const std::string& caption);

		bool GetAndResetClickStatus(const std::string& caption);
		ClickMenuItem* GetMenuItem(const std::string& caption);
		ClickMenu* GetSubMenu(const std::string& caption);
	
	private:
		typedef std::map<std::string, ClickMenuItem*> ItemMap;

		ID3D10Device*						mDevice;
		ItemMap								mItems;
		int									mItemWidth;
		int									mItemHeight;
	};
}
#endif
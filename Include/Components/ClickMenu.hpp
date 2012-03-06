#ifndef CLICKMENU_HPP
#define CLICKMENU_HPP

#include <map>
#include "GameFont.hpp"
#include "ComponentGroup.hpp"
#include "Button.hpp"

namespace Components
{
	class ClickMenu;

	namespace ClickMenuState
	{
		enum ClickMenuState { Collapsed, Opening, Collapsing, Opened };
	}

	class ClickMenuItem : public Button
	{
	public:
		ClickMenuItem(ClickMenu* ownerGroup);
		~ClickMenuItem() throw();

		void AddSubItem(const std::string& caption);
		ClickMenu* GetSubMenu();

		void Initialize(ID3D10Device* device, RECT position, const std::string& caption);
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();

		// DEBUG
		virtual std::string GetName();
	private:
		ClickMenu*						mSubMenu;
		ClickMenuState::ClickMenuState	mSubMenuState;
		double							mSubMenuInterpolation;

		GameFont*						mFont;
		std::string						mCaption;
		D3DXCOLOR						mTextColor;
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
		void SetPosition(const POINT& position);

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
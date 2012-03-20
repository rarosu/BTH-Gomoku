#ifndef CLICKMENU_HPP
#define CLICKMENU_HPP

#include <vector>
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
		std::string GetCaption();
		void SetCaption(std::string caption);
		bool GetAndResetClickStatus();

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

	class LazyClickMenuItem : public ClickMenuItem
	{
	public:
		LazyClickMenuItem(ClickMenu* ownerGroup, RECT position);

	protected:
		virtual void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState) {}
	};

	class ClickMenu : public ComponentGroup
	{
	public:
		ClickMenu(ComponentGroup* ownerGroup, 
				  ID3D10Device* device, 
				  RECT position, 
				  int itemWidth, 
				  int itemHeight,
				  bool isLazy = false);

		void AddMenuItem(const std::string& caption);
		void LostFocus();

		bool GetAndResetClickStatus(const std::string& caption);
		bool GetAndResetClickStatus(const int index);
		ClickMenuItem* GetMenuItem(const std::string& caption);
		ClickMenuItem* GetMenuItem(const int index);
		ClickMenu* GetSubMenu(const std::string& caption);
		ClickMenu* GetSubMenu(const int index);
		void Collapse(const int index);
		void Collapse(ClickMenuItem* collapseItem);
		void CollapseAll();
		void Expand(const int index);
		void Expand(ClickMenuItem* collapseItem);
	
	private:
		ID3D10Device*						mDevice;
		std::vector<ClickMenuItem*>			mItems;
		int									mItemWidth;
		int									mItemHeight;
		bool								mIsLazy;
	};
}
#endif
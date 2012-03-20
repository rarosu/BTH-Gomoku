#include "ClickMenu.hpp"
#include <cassert>

// Menu Item
namespace Components
{
	ClickMenuItem::ClickMenuItem(ClickMenu* ownerGroup, RECT position)
		: Button(ownerGroup, position),
		  mSubMenu(NULL), 
		  mFont (NULL), 
		  mTextColor(D3DXCOLOR(1.0, 1.0, 1.0, 1.0))
	{}

	ClickMenuItem::~ClickMenuItem() throw()
	{
		SafeDelete(mFont);
	}

	void ClickMenuItem::Initialize(ID3D10Device* device, const std::string& caption)
	{
		Graphics buttonGraphics;
		buttonGraphics.activeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		buttonGraphics.idleColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		buttonGraphics.hoverColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
		buttonGraphics.textureUp = new Sprite(device, sViewport, "buttonBase.png", GetWidth(), GetHeight());

		Button::Initialize(device, buttonGraphics);

		int textSize = GetHeight() / 2;
		mFont = new GameFont(device, "Jing Jing", textSize, false, true);
		mCaption = caption;
	}

	void ClickMenuItem::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if (mSubMenu != NULL)
		{
			if (GetAndResetClickStatus())
			{
				if(mSubMenu->IsVisible())
					reinterpret_cast<ClickMenu*>(mOwner)->Collapse(this);
				else
					reinterpret_cast<ClickMenu*>(mOwner)->Expand(this);
			}
		}
	}

	void ClickMenuItem::Draw()
	{
		if(!IsVisible())
			return;

		Button::Draw();

		D3DXVECTOR2 pos = GetPosition();
		RECT posRect = { pos.x, pos.y, pos.x + GetWidth(), pos.y + GetHeight() };

		if(IsEnabled())
		{
			if(IsHovered())
				mFont->WriteText(mCaption, &posRect, D3DXCOLOR(1.0, 1.0, 0.0, 1.0), GameFont::Center, GameFont::Middle);
			else
				mFont->WriteText(mCaption, &posRect, mTextColor, GameFont::Center, GameFont::Middle);
		}
		else
			mFont->WriteText(mCaption, &posRect, D3DXCOLOR(0.5, 0.5, 0.5, 1.0), GameFont::Center, GameFont::Middle);
	}

	std::string ClickMenuItem::GetCaption()
	{
		return mCaption;
	}

	void ClickMenuItem::SetCaption(std::string caption)
	{
		mCaption = caption;
	}

	bool ClickMenuItem::GetAndResetClickStatus()
	{
		bool result = Button::GetAndResetClickStatus();

		/*if(result)
			reinterpret_cast<ClickMenu*>(mOwner)->Collapse();*/

		return result;
	}

	void ClickMenuItem::AddSubItem(const std::string& caption)
	{
		if(mSubMenu == NULL)
		{
			int width = GetWidth();
			int height = GetHeight();
			D3DXVECTOR2 position = GetPosition() - mOwner->GetPosition();
			RECT subPos = { width, position.y, width * 2, position.y + height };
			mSubMenu = new ClickMenu(mOwner, mDevice, subPos, width, height);
			reinterpret_cast<ClickMenu*>(mOwner)->Collapse(this);
		}

		mSubMenu->AddMenuItem(caption);
	}

	// DEBUG
	std::string ClickMenuItem::GetName()
	{
		return "MenuItem: " + mCaption;
	}

	ClickMenu* ClickMenuItem::GetSubMenu()
	{
		return mSubMenu;
	}

}

// Lazy Click Menu Item
namespace Components
{
	LazyClickMenuItem::LazyClickMenuItem(ClickMenu* ownerGroup, RECT position)
		: ClickMenuItem(ownerGroup, position)
	{}
}

// Click Menu
namespace Components
{
	ClickMenu::ClickMenu(ComponentGroup* ownerGroup, ID3D10Device* device, RECT position, 
						 int itemWidth, int itemHeight, bool isLazy)
		: ComponentGroup(ownerGroup, "ClickMenu", position),
		  mDevice(device), mItemWidth(itemWidth), mItemHeight(itemHeight), mIsLazy(isLazy)
	{
	}

	void ClickMenu::AddMenuItem(const std::string& caption)
	{
		LONG yTop = (mItemHeight * mItems.size());
		LONG yBottom = yTop + mItemHeight;
		RECT pos = { 0, yTop, mItemWidth, yBottom };
		
		ClickMenuItem* newItem = NULL;
		if(mIsLazy)
			newItem = new LazyClickMenuItem(this, pos);
		else
			newItem = new ClickMenuItem(this, pos);

		newItem->Initialize(mDevice, caption);
		
		RECT menuRect = GetBoundingRect();
		menuRect.bottom += mItemHeight;

		mItems.push_back(newItem);
		SetBoundingRect(menuRect);
	}
	
	void ClickMenu::LostFocus()
	{
		CollapseAll();
		ComponentGroup::LostFocus();
	}

	bool ClickMenu::GetAndResetClickStatus(const std::string& caption)
	{
		return GetAndResetClickStatus(GetIndex(caption));
	}

	bool ClickMenu::GetAndResetClickStatus(const int index)
	{
		if(index >= 0 && index < mItems.size())
		{
			ClickMenuItem* currItem = reinterpret_cast<ClickMenuItem*>(mItems[index]);
			bool clickStatus = currItem->GetAndResetClickStatus();

			ClickMenu* subMenu = currItem->GetSubMenu();
			if(subMenu != NULL)
			{
				for(int i = 0; i < subMenu->GetItemCount(); ++i)
				{
					subMenu->GetAndResetClickStatus(i);
				}
			}

			return clickStatus;
		}
		else
			return false;
	}

	ClickMenuItem* ClickMenu::GetMenuItem(const std::string& caption)
	{
		return GetMenuItem(GetIndex(caption));
	}

	ClickMenuItem* ClickMenu::GetMenuItem(const int index)
	{
		if(index >= 0 && index < mItems.size())
			return reinterpret_cast<ClickMenuItem*>(mItems[index]);
		else
			return NULL;
	}

	ClickMenu* ClickMenu::GetSubMenu(const int index)
	{
		if(index >= 0 && index < mItems.size())
			return reinterpret_cast<ClickMenuItem*>(mItems[index])->GetSubMenu();
		else
			return NULL;
	}

	ClickMenu* ClickMenu::GetSubMenu(const std::string& caption)
	{
		return GetSubMenu(GetIndex(caption));
	}

	void ClickMenu::Collapse(const int index)
	{
		assert(index >= 0);
		assert(index < mItems.size());

		ClickMenuItem* currItem = reinterpret_cast<ClickMenuItem*>(mItems[index]);
		Collapse(currItem);
	}

	void ClickMenu::Collapse(ClickMenuItem* collapseItem)
	{
		ClickMenu* subMenu = collapseItem->GetSubMenu();
		if(subMenu != NULL)
			subMenu->SetVisible(false);
	}

	void ClickMenu::CollapseAll()
	{
		for(int i = 0; i < mItems.size(); ++i)
		{
			Collapse(mItems[i]);
		}
	}

	void ClickMenu::Expand(const int index)
	{
		assert(index >= 0);
		assert(index < mItems.size());

		ClickMenuItem* currItem = reinterpret_cast<ClickMenuItem*>(mItems[index]);
		Expand(currItem);
	}

	void ClickMenu::Expand(ClickMenuItem* collapseItem)
	{
		CollapseAll();
		ClickMenu* subMenu = collapseItem->GetSubMenu();
		if(subMenu != NULL)
			subMenu->SetVisible(true);
	}

	int ClickMenu::GetIndex(std::string caption)
	{
		for(int i = 0; i < mItems.size(); ++i)
		{
			if(mItems[i]->GetCaption() == caption)
				return i;
		}

		return -1;
	}

	int ClickMenu::GetItemCount() const
	{
		return mItems.size();
	}
}
#include "ClickMenu.hpp"

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
					mSubMenu->SetVisible(false);
				else
					mSubMenu->SetVisible(true);
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

	void ClickMenuItem::AddSubItem(const std::string& caption)
	{
		if(mSubMenu == NULL)
		{
			int width = GetWidth();
			int height = GetHeight();
			RECT subPos = GetBoundingRect();
			subPos.left = subPos.right;
			subPos.right += width;
			mSubMenu = new ClickMenu(mOwner, mDevice, subPos, width, height);
			mSubMenu->SetVisible(false);
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

// Click Menu
namespace Components
{
	ClickMenu::ClickMenu(ComponentGroup* ownerGroup, ID3D10Device* device, RECT position, 
						 int itemWidth, int itemHeight)
		: ComponentGroup(ownerGroup, "ClickMenu", position),
		  mDevice(device), mItemWidth(itemWidth), mItemHeight(itemHeight)
	{
	}

	void ClickMenu::AddMenuItem(const std::string& caption)
	{
		LONG yTop = (mItemHeight * mItems.size());
		LONG yBottom = yTop + mItemHeight;
		RECT pos = { 0, yTop, mItemWidth, yBottom };

		ClickMenuItem* newItem = new ClickMenuItem(this, pos);
		newItem->Initialize(mDevice, caption);
		
		mItems[caption] = newItem;
	}

	bool ClickMenu::GetAndResetClickStatus(const std::string& caption)
	{
		ItemMap::iterator it = mItems.find(caption);

		if (it != mItems.end())
			return it->second->GetAndResetClickStatus();
		
		return false;
	}

	ClickMenuItem* ClickMenu::GetMenuItem(const std::string& caption)
	{
		ItemMap::iterator it = mItems.find(caption);

		if (it != mItems.end())
			return it->second;

		return NULL;
	}

	ClickMenu* ClickMenu::GetSubMenu(const std::string& caption)
	{
		ItemMap::iterator it = mItems.find(caption);

		if (it != mItems.end())
			return it->second->GetSubMenu();

		return NULL;
	}
}
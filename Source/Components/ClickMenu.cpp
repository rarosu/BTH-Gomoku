#include "ClickMenu.hpp"

// Menu Item
namespace Components
{
	MenuItem::MenuItem(ClickMenu* ownerGroup)
		: Button(ownerGroup),
		  mSubMenu(NULL), mFont (NULL), mTextColor(D3DXCOLOR(1.0, 1.0, 1.0, 1.0))
	{
	}

	MenuItem::~MenuItem() throw()
	{
		SafeDelete(mFont);
	}

	void MenuItem::Initialize(ID3D10Device* device, RECT position, const std::string& caption)
	{
		Graphics buttonGraphics;
		buttonGraphics.activeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		buttonGraphics.idleColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		buttonGraphics.hoverColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/buttonBase.png", NULL, NULL, 
											   &buttonGraphics.textureUp, NULL);

		Button::Initialize(device, position, buttonGraphics);

		int textSize = (mPositionRect.bottom - mPositionRect.top) / 2;
		mFont = new GameFont(device, "Jing Jing", textSize, false, true);
		mCaption = caption;
	}

	void MenuItem::AddSubItem(const std::string& caption)
	{
		if(mSubMenu == NULL)
		{
			int width = mPositionRect.right - mPositionRect.left;
			int height = mPositionRect.bottom - mPositionRect.top;
			POINT subPos = { mPositionRect.right, mPositionRect.top };
			mSubMenu = new ClickMenu(mOwner, mDevice, subPos, width, height);
		}

		mSubMenu->AddMenuItem(caption);
	}

	void MenuItem::Draw()
	{
		Button::Draw();
		if(IsEnabled())
		{
			if(IsHovered())
				mFont->WriteText(mCaption, &mPositionRect, D3DXCOLOR(1.0, 1.0, 0.0, 1.0), GameFont::Center, GameFont::Middle);
			else
				mFont->WriteText(mCaption, &mPositionRect, mTextColor, GameFont::Center, GameFont::Middle);
		}
		else
			mFont->WriteText(mCaption, &mPositionRect, D3DXCOLOR(0.5, 0.5, 0.5, 1.0), GameFont::Center, GameFont::Middle);
	}

	// DEBUG
	std::string MenuItem::GetName()
	{
		return "MenuItem: " + mCaption;
	}

	ClickMenu* MenuItem::GetSubMenu()
	{
		return mSubMenu;
	}

}

// Click Menu
namespace Components
{
	ClickMenu::ClickMenu(ComponentGroup* ownerGroup, ID3D10Device* device, const POINT& position, 
						 int itemWidth, int itemHeight)
		: ComponentGroup(ownerGroup, "ClickMenu"),
		  mDevice(device), mItemWidth(itemWidth), mItemHeight(itemHeight)
	{
		RECT pos = { position.x, position.y, position.x + mItemWidth, position.y };
		mPositionRect = pos;
	}

	void ClickMenu::AddMenuItem(const std::string& caption)
	{
		MenuItem* newItem = new MenuItem(this);
		LONG yTop = mPositionRect.top + (mItemHeight * mItems.size());
		LONG yBottom = yTop + mItemHeight;
		RECT pos = { mPositionRect.left, yTop, mPositionRect.right, yBottom };
		newItem->Initialize(mDevice, pos, caption);
		
		mItems[caption] = newItem;
		mPositionRect.bottom = yBottom;
	}

	bool ClickMenu::GetAndResetClickStatus(const std::string& caption)
	{
		ItemMap::iterator it = mItems.find(caption);

		if (it != mItems.end())
			return it->second->GetAndResetClickStatus();
		
		return false;
	}

	MenuItem* ClickMenu::GetMenuItem(const std::string& caption)
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
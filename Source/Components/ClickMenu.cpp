#include "ClickMenu.hpp"

// Menu Item
namespace Components
{
	ClickMenuItem::ClickMenuItem(ClickMenu* ownerGroup)
		: Button(ownerGroup),
		  mSubMenu(NULL), 
		  mFont (NULL), 
		  mTextColor(D3DXCOLOR(1.0, 1.0, 1.0, 1.0)),
		  mSubMenuState(ClickMenuState::Collapsed),
		  mSubMenuInterpolation(0.0f)
	{
	}

	ClickMenuItem::~ClickMenuItem() throw()
	{
		SafeDelete(mFont);
	}

	void ClickMenuItem::Initialize(ID3D10Device* device, RECT position, const std::string& caption)
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

	void ClickMenuItem::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		Button::Update(gameTime, currInputState, prevInputState);

		if (mSubMenu != NULL)
		{
			if (GetAndResetClickStatus())
			{
				if (mSubMenuState == ClickMenuState::Collapsed || mSubMenuState == ClickMenuState::Collapsing)
				{
					mSubMenuState = ClickMenuState::Opening;
				}
				else
				{
					mSubMenuState = ClickMenuState::Collapsing;
				}
			}
		}
	}

	void ClickMenuItem::Draw()
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



	void ClickMenuItem::AddSubItem(const std::string& caption)
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
		ClickMenuItem* newItem = new ClickMenuItem(this);
		LONG yTop = mPositionRect.top + (mItemHeight * mItems.size());
		LONG yBottom = yTop + mItemHeight;
		RECT pos = { mPositionRect.left, yTop, mPositionRect.right, yBottom };
		newItem->Initialize(mDevice, pos, caption);
		
		mItems[caption] = newItem;
		mPositionRect.bottom = yBottom;
	}

	void ClickMenu::SetPosition(const POINT& position)
	{
		RECT pos = { position.x, position.y, position.x + mItemWidth, position.y + (mPositionRect.bottom - mPositionRect.top) };
		mPositionRect = pos;
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
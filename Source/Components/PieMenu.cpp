#include "PieMenu.hpp"
#include <cmath>
#include <sstream>

namespace Components
{
	const int			PieMenuItem::C_ITEM_SIZE = 64;

	PieMenuItem::PieMenuItem(ComponentGroup* ownerGroup, RECT position, ID3D10Device* device,
							 std::string idleSpriteFilename, std::string hoveredSpriteFilename)
		: Component(ownerGroup, position),
		  mIsSelected(false)
	{
		mIdleSprite = new Sprite(device, sViewport, idleSpriteFilename, C_ITEM_SIZE, C_ITEM_SIZE);
		mHoverSprite = new Sprite(device, sViewport, hoveredSpriteFilename, C_ITEM_SIZE, C_ITEM_SIZE);
	}

	PieMenuItem::~PieMenuItem() throw()
	{
		SafeDelete(mIdleSprite);
		SafeDelete(mHoverSprite);
	}

	bool PieMenuItem::IsSelected()
	{
		return mIsSelected;
	}

	void PieMenuItem::SetSelected(bool isSelected)
	{
		mIsSelected = isSelected;
	}

	void PieMenuItem::Draw()
	{
		if(mIsSelected)
		{
			mHoverSprite->Draw(GetPosition());
		}
		else
		{
			mIdleSprite->Draw(GetPosition());
		}
	}
	
	void PieMenuItem::LostFocus()
	{
	}
	
	void PieMenuItem::GotFocus()
	{
	}

	void PieMenuItem::MouseEntered()
	{
	}
	
	void PieMenuItem::MouseExited()
	{
	}
	
	void PieMenuItem::MousePressed(int buttonIndex, const InputState& currentState)
	{
	}
	
	void PieMenuItem::MouseReleased(int buttonIndex, const InputState& currentState)
	{
	}
	
	void PieMenuItem::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
	}

	std::string PieMenuItem::GetName()
	{
		return "PieMenuItem";
	}
}

namespace Components
{
	PieMenu::PieMenu(ComponentGroup* ownerGroup, ID3D10Device* device, RECT position)
		: ComponentGroup(ownerGroup, "Piemenu", position),
		  mDevice(device), mDefaultFont(NULL), mMinDist(30), mMaxDist(200), mClickedItemIndex(-1)
	{
		float vectorLength = PieMenuItem::C_ITEM_SIZE;
		D3DXVECTOR2 centerOffset = D3DXVECTOR2(-(PieMenuItem::C_ITEM_SIZE / 2), -(PieMenuItem::C_ITEM_SIZE / 2));

		std::string idleTextures[] = { "question.png", "exclamation.png", "hint.png", "other.png" };
		std::string hoverTextures[] = { "questionL.png", "exclamationL.png", "hintL.png", "otherL.png" };

		for(int i = 0; i < C_ITEM_COUNT; ++i)
		{
			float angle = i * (-2 * D3DX_PI / C_ITEM_COUNT);
			D3DXVECTOR2 centerPosition = D3DXVECTOR2(std::cos(angle) * vectorLength, std::sin(angle) * vectorLength);
			RECT newPos = { centerPosition.x + centerOffset.x, centerPosition.y + centerOffset.y, 0, 0 };
			Components::PieMenuItem* item = new Components::PieMenuItem(this, newPos, mDevice, idleTextures[i],
				hoverTextures[i]);
		}

		SetVisible(false);

		// DEBUG
		mDefaultFont = new GameFont(mDevice, "Arial", 21);
	}

	PieMenu::~PieMenu() throw()
	{
		SafeDelete(mDefaultFont);
	}

	int PieMenu::GetAndResetClickedItemIndex()
	{
		int result = -1;

		if(!IsVisible())
		{
			result = mClickedItemIndex;
			mClickedItemIndex = -1;
		}

		return result;
	}

	void PieMenu::MouseButtonPressed(int buttonIndex, const InputState& currentState)
	{
		if (!IsEnabled())
			return;

		if(buttonIndex == C_MOUSE_RIGHT)
		{
			SetVisible(true);
			SetPosition(D3DXVECTOR2(currentState.Mouse.x, currentState.Mouse.y));
		}
	}

	void PieMenu::MouseButtonReleased(int buttonIndex, const InputState& currentState)
	{
		if(buttonIndex == C_MOUSE_RIGHT)
		{
			SetVisible(false);
			mClickedItemIndex = -1;

			for(int i = 0; i < C_ITEM_COUNT; ++i)
			{
				PieMenuItem* currItem = static_cast<PieMenuItem*>(mComponents[i]);
				if(currItem->IsSelected())
					mClickedItemIndex = i;
			}
		}
	}

	void PieMenu::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		ComponentGroup::Refresh(gameTime, currInputState, prevInputState);

		D3DXVECTOR2 mousePos = D3DXVECTOR2(currInputState.Mouse.x, currInputState.Mouse.y);
		D3DXVECTOR2 centerToMouseVec = GetPosition() - mousePos;
		float distFromCenter = D3DXVec2Length(&centerToMouseVec);

		if(distFromCenter < mMinDist || distFromCenter > mMaxDist)
		{
			// Set all items to not selected
			for(int i = 0; i < C_ITEM_COUNT; ++i)
			{
				PieMenuItem* currItem = static_cast<PieMenuItem*>(mComponents[i]);
				currItem->SetSelected(false);
			}

			return;
		}

		float angleToX = std::atan2(-centerToMouseVec.y, centerToMouseVec.x) + D3DX_PI;
		float itemAngle = (2 * D3DX_PI / C_ITEM_COUNT);
		angleToX += itemAngle / 2;

		if(angleToX > (D3DX_PI * 2))		// Make sure angle is in the interval [0, 2 * PI]
			angleToX = angleToX - (D3DX_PI * 2);

		for(int i = 0; i < C_ITEM_COUNT; ++i)
		{
			PieMenuItem* currItem = static_cast<PieMenuItem*>(mComponents[i]);

			if(angleToX > i * itemAngle && angleToX < (i + 1) * itemAngle)
				currItem->SetSelected(true);
			else
				currItem->SetSelected(false);
		}
	}

	void PieMenu::Draw()
	{
		ComponentGroup::Draw();

		/*POINT textPos = { GetPosition().x - 100, GetPosition().y + 100 };
		mDefaultFont->WriteText(mOutput, textPos, D3DXCOLOR(0.0, 0.0, 1.0, 1.0));*/
	}
}
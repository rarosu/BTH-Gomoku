#include "PieMenu.hpp"

namespace Components
{
	PieMenuItem::PieMenuItem(ComponentGroup* ownerGroup, RECT position, ID3D10Device* device,
							 std::string idleSpriteFilename, std::string hoveredSpriteFilename)
		: Component(ownerGroup, position),
		  mIsSelected(false)
	{
		mIdleSprite = new Sprite(device, sViewport, idleSpriteFilename, 64, 64);
		mHoverSprite = new Sprite(device, sViewport, hoveredSpriteFilename, 64, 64);
	}

	bool PieMenuItem::GetAndResetClickStatus(const std::string& caption)
	{
		return false;
	}

	void PieMenuItem::SetSelected(bool isSelected)
	{
		mIsSelected = isSelected;
	}

	void PieMenuItem::Draw()
	{
		/*if(mIsSelected)
			mHoverSprite->Draw();
		else
			mIdleSprite->Draw();*/
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
	PieMenu::PieMenu(ComponentGroup* ownerGroup, RECT position)
		: ComponentGroup(ownerGroup, "Piemenu", position)
	{
	}

	void PieMenu::AddItem(std::string idleSpriteFilename, std::string hoveredSpriteFilename)
	{

	}

	void PieMenu::MousePressed(int buttonIndex, const InputState& currentState)
	{
		if(buttonIndex == C_MOUSE_RIGHT)
		{
			SetVisible(true);
			float halfWidth = GetWidth() / 2;
			float halfHeight = GetHeight() / 2;
			SetPosition(D3DXVECTOR2(currentState.Mouse.x - halfWidth, currentState.Mouse.y - halfHeight));
		}
	}

	void PieMenu::MouseReleased(int buttonIndex, const InputState& currentState)
	{
		if(buttonIndex == C_MOUSE_RIGHT)
			SetVisible(false);
	}
}
#include "TextButton.hpp"

namespace Components
{
	TextButton::TextButton(ComponentGroup* ownerGroup, RECT position)
		: Button(ownerGroup, position),
		  mFont (NULL), mTextColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))
	{
	}

	void TextButton::Initialize(ID3D10Device* device, std::string caption)
	{
		Graphics buttonGraphics;
		buttonGraphics.activeColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		buttonGraphics.idleColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		buttonGraphics.hoverColor = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
		buttonGraphics.textureUp = new Sprite(device, sViewport, "buttonUp.png", GetWidth(), GetHeight());
		buttonGraphics.textureDown = new Sprite(device, sViewport, "buttonDown.png", GetWidth(), GetHeight());


		Button::Initialize(device, buttonGraphics);

		int textSize = GetHeight() / 2;
		mFont = new GameFont(device, "Jing Jing", textSize, false, true);
		mCaption = caption;
	}

	void TextButton::Draw()
	{
		if(!IsVisible())
			return;

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
	std::string TextButton::GetName()
	{
		return "Button: " + mCaption;
	}
}
#include "TextButton.hpp"

namespace Components
{
	TextButton::TextButton(ComponentGroup* ownerGroup)
		: Button(ownerGroup),
		  mFont (NULL), mTextColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))
	{
	}

	void TextButton::Initialize(ID3D10Device* device, RECT position, std::string caption)
	{
		Graphics buttonGraphics;
		buttonGraphics.activeColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		buttonGraphics.idleColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		buttonGraphics.hoverColor = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

		int width = position.right - position.left;
		int height = position.bottom - position.top;

		buttonGraphics.textureUp = new Sprite(device, sViewport, "buttonUp.png", width, height);
		buttonGraphics.textureDown = new Sprite(device, sViewport, "buttonDown.png", width, height);

		/*D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/buttonUp.png", NULL, NULL, 
											   &buttonGraphics.textureUp, NULL);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/buttonDown.png", NULL, NULL, 
											   &buttonGraphics.textureDown, NULL);*/

		Button::Initialize(device, position, buttonGraphics);

		int textSize = (mPositionRect.bottom - mPositionRect.top) / 2;
		mFont = new GameFont(device, "Jing Jing", textSize, false, true);
		mCaption = caption;
	}

	void TextButton::Draw()
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
	std::string TextButton::GetName()
	{
		return "Button: " + mCaption;
	}
}
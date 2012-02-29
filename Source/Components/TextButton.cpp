#include "TextButton.hpp"

namespace Components
{
	TextButton::TextButton(ComponentGroup* ownerGroup)
		: Button(ownerGroup),
		  mFont (NULL), mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0))
	{
	}

	void TextButton::Initialize(ID3D10Device* device, RECT position, std::string caption)
	{
		Graphics buttonGraphics;
		buttonGraphics.hoverColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/buttonUp.png", NULL, NULL, 
											   &buttonGraphics.textureUp, NULL);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/buttonDown.png", NULL, NULL, 
											   &buttonGraphics.textureDown, NULL);

		Button::Initialize(device, position, buttonGraphics);

		int textSize = (mPositionRect.bottom - mPositionRect.top) / 2;
		mFont = new GameFont(device, "Jing Jing", textSize, false, true);
		mCaption = caption;
	}

	void TextButton::Draw()
	{
		Button::Draw();
		mFont->WriteText(mCaption, &mPositionRect, mTextColor, GameFont::Center, GameFont::Middle);
	}

	// DEBUG
	std::string TextButton::GetName()
	{
		return "Button: " + mCaption;
	}
}
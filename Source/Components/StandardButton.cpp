#include "StandardButton.hpp"

namespace Components
{
	StandardButton::StandardButton(InputSubscription* manager)
		: Button(manager),
		mFont (NULL), mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0))
	{
	}

	void StandardButton::Initialize(ID3D10Device* device, RECT position, std::string caption)
	{
		Graphics buttonGraphics;
		buttonGraphics.hoverColor = D3DXCOLOR(0.8f, 0.8f, 0.0f, 1.0f);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/buttonUp.png", NULL, NULL, 
											   &buttonGraphics.textureUp, NULL);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/buttonDown.png", NULL, NULL, 
											   &buttonGraphics.textureDown, NULL);
		
		Button::Initialize(device, position, buttonGraphics);

		mFont = new GameFont(device, "Comic Sans", 18, false, true);
		mCaption = caption;
	}

	void StandardButton::Draw()
	{
		Button::Draw();
		mFont->WriteText(mCaption, &mPositionRect, mTextColor, GameFont::Center, GameFont::Middle);
	}
}
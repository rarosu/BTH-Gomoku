#include "Button.hpp"

namespace Components
{
	Button::Graphics::Graphics()
	{
		idleColor = activeColor = hoverColor = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
		textureUp = NULL;
		textureDown = NULL;
		textureHover = NULL;
	}

	Button::Button(ComponentGroup* ownerGroup)
		: Clickable(ownerGroup),
		mDevice(NULL), mTexture(NULL), mTintColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
	{
	}

	void Button::Initialize(ID3D10Device* device, RECT position, Graphics buttonGraphics)
	{
		mDevice = device;
		mGraphics = buttonGraphics;
		mPositionRect = position;

		if(mGraphics.textureUp == NULL)
		{
			mGraphics.textureUp = new Sprite(mDevice, sViewport, "button.png", GetWidth(), GetHeight());
		}

		mTexture = mGraphics.textureUp;
		mTintColor = mGraphics.idleColor;
	}

	void Button::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(IsEnabled())
			Clickable::Update(gameTime, currInputState, prevInputState);
	}

	void Button::Draw()
	{
		if(mTexture != NULL)
		{
			D3DXVECTOR2 position = D3DXVECTOR2(mPositionRect.left, mPositionRect.top);
			mTexture->Draw(position, mTintColor);
		}
	}
	
	void Button::LostFocus()
	{
	}

	void Button::GotFocus()
	{
	}

	void Button::MouseEntered()
	{
		if(IsPressed())
		{
			mTintColor = mGraphics.activeColor;
			if(mGraphics.textureDown)
				mTexture = mGraphics.textureDown;
		}
		else
		{
			mTintColor = mGraphics.hoverColor;
			if(mGraphics.textureHover)
				mTexture = mGraphics.textureHover;

		}
	}

	void Button::MouseExited()
	{
		mTintColor = mGraphics.idleColor;
		mTexture = mGraphics.textureUp;
	}

	void Button::MousePressed(int buttonIndex)
	{
		mTintColor = mGraphics.activeColor;
		if(mGraphics.textureDown)
			mTexture = mGraphics.textureDown;
	}

	void Button::MouseReleased(int buttonIndex)
	{
		mTintColor = mGraphics.hoverColor;
		if(mGraphics.textureHover)
			mTexture = mGraphics.textureHover;
		else
			mTexture = mGraphics.textureUp;
	}

	std::string Button::GetName()
	{
		return "Button";
	}
}
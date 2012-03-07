#include "Scrollbar.hpp"

namespace Components
{
	Scrollbar::Scrollbar(ComponentGroup* ownerGroup, Scrollable* scrollable)
		: Clickable(ownerGroup), 
		  mScrollable(scrollable), mBGColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), mBtnUp(NULL), mBtnDown(NULL)
	{
		mBtnUp = new Button(ownerGroup);
		mBtnDown = new Button(ownerGroup);
	}

	Scrollbar::~Scrollbar() throw()
	{
		SafeDelete(mBackground);
	}

	void Scrollbar::Initialize(ID3D10Device* device, RECT position)
	{
		mDevice = device;
		mPositionRect = position;
		LONG buttonHeight = position.right - position.left;	// Button height is same as its width

		mBackground = new Sprite(mDevice, sViewport, "whitePixel.png", GetWidth(), GetHeight() - buttonHeight);

		Button::Graphics graphics1, graphics2;
		graphics1.textureUp = new Sprite(device, sViewport, "btnUIdle.png", buttonHeight, buttonHeight);
		graphics1.textureDown = new Sprite(device, sViewport, "btnUPressed.png", buttonHeight, buttonHeight);
		graphics2.textureUp = new Sprite(device, sViewport, "btnDIdle.png", buttonHeight, buttonHeight);
		graphics2.textureDown = new Sprite(device, sViewport, "btnDPressed.png", buttonHeight, buttonHeight);

		RECT pos1 = { position.left, position.top, position.right, position.top + buttonHeight };

		RECT pos2 = { position.left, position.bottom - buttonHeight, position.right, position.bottom };
		
		mBtnUp->Initialize(device, pos1, graphics1);
		mBtnDown->Initialize(device, pos2, graphics2);

		mBGColor = C_COLOR_COMPONENT_BG;
		mBGDrawOffset = buttonHeight / 2;
	}

	void Scrollbar::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		Clickable::Update(gameTime, currInputState, prevInputState);

		mBtnUp->Update(gameTime, currInputState, prevInputState);
		mBtnDown->Update(gameTime, currInputState, prevInputState);
	}

	void Scrollbar::Draw()
	{
		if(mBackground != NULL)
		{
			D3DXVECTOR2 position = D3DXVECTOR2(mPositionRect.left, mPositionRect.top + mBGDrawOffset);
			mBackground->Draw(position, mBGColor);
		}

		mBtnUp->Draw();
		mBtnDown->Draw();
	}

	void Scrollbar::LostFocus()
	{
	}
	
	void Scrollbar::GotFocus()
	{
	}

	void Scrollbar::MouseEntered()
	{
	}

	void Scrollbar::MouseExited()
	{
	}

	void Scrollbar::MousePressed(int buttonIndex)
	{
	}

	void Scrollbar::MouseReleased(int buttonIndex)
	{
		if(mBtnUp->IsHovered() && mScrollable)
			mScrollable->Scroll(true);
		else if(mBtnDown->IsHovered() && mScrollable)
			mScrollable->Scroll(false);
	}

	// DEBUG
	std::string Scrollbar::GetName()
	{
		return "Scrollbar";
	}
}
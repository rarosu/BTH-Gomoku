#include "Scrollbar.hpp"

namespace Components
{
	Scrollbar::Scrollbar(ComponentGroup* ownerGroup, Scrollable* scrollable, RECT position)
		: Component(ownerGroup, position), 
		  mScrollable(scrollable), mBGColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), mBtnUp(NULL), mBtnDown(NULL)
	{
		LONG buttonHeight = GetWidth();
		D3DXVECTOR2 scrollPos = GetPosition();
		RECT pos1 = { scrollPos.x, scrollPos.y, scrollPos.x + GetWidth(), scrollPos.y + buttonHeight };
		RECT pos2 = { scrollPos.x, scrollPos.y - buttonHeight, scrollPos.x + GetWidth(), scrollPos.y };

		mBtnUp = new Button(ownerGroup, pos1);
		mBtnDown = new Button(ownerGroup, pos2);
	}

	Scrollbar::~Scrollbar() throw()
	{
		SafeDelete(mBackground);
	}

	void Scrollbar::Initialize(ID3D10Device* device)
	{
		mDevice = device;
		LONG buttonHeight = GetWidth();	// Button height is same as its width

		mBackground = new Sprite(mDevice, sViewport, "whitePixel.png", GetWidth(), GetHeight() - buttonHeight);

		Button::Graphics graphics1, graphics2;
		graphics1.textureUp = new Sprite(device, sViewport, "btnUIdle.png", buttonHeight, buttonHeight);
		graphics1.textureDown = new Sprite(device, sViewport, "btnUPressed.png", buttonHeight, buttonHeight);
		graphics2.textureUp = new Sprite(device, sViewport, "btnDIdle.png", buttonHeight, buttonHeight);
		graphics2.textureDown = new Sprite(device, sViewport, "btnDPressed.png", buttonHeight, buttonHeight);
		
		mBtnUp->Initialize(device, graphics1);
		mBtnDown->Initialize(device, graphics2);

		mBGColor = C_COLOR_COMPONENT_BG;
		mBGDrawOffset = buttonHeight / 2;
	}

	void Scrollbar::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		mBtnUp->Update(gameTime, currInputState, prevInputState);
		mBtnDown->Update(gameTime, currInputState, prevInputState);
	}

	void Scrollbar::Draw()
	{
		if(!IsVisible())
			return;

		if(mBackground != NULL)
		{
			D3DXVECTOR2 position = GetPosition() + D3DXVECTOR2(0, mBGDrawOffset);
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
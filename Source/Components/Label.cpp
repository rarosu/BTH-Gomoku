#include "Label.hpp"

namespace Components
{
	Label::Label(ID3D10Device* device, ComponentGroup* ownerGroup, std::string caption, RECT position, 
				 int textSize, GameFont::AlignHorizontal alignHorizontal, GameFont::AlignVertical alignVertical)
		: Component(ownerGroup, position),
		  mFont(NULL), mCaption(caption), mTextColor(C_COLOR_TEXT), 
		  mShadowColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)), mAlignHor(alignHorizontal), mAlignVer(alignVertical)
	{
		D3DXVECTOR2 lblPos = GetPosition();
		int offsetX = 2;
		int offsetY = 1;
		mShadowPosition.left = lblPos.x + offsetX;
		mShadowPosition.right = lblPos.x + GetWidth() + offsetX;
		mShadowPosition.top = lblPos.y + offsetY;
		mShadowPosition.bottom = lblPos.y + GetHeight() + offsetY;

		if(textSize <= 0)
			textSize = static_cast<int>(GetHeight());

		mFont = new GameFont(device, "Segoe Print", textSize, false, true);
	}

	void Label::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
	}

	void Label::Draw()
	{
		if(!IsVisible())
			return;

		RECT textPos = GetBoundingRect();
		mFont->WriteText(mCaption, &mShadowPosition, mShadowColor, mAlignHor, mAlignVer);
		mFont->WriteText(mCaption, &textPos, mTextColor, mAlignHor, mAlignVer);
	}

	void Label::LostFocus()
	{
	}
	
	void Label::GotFocus()
	{
	}

	std::string Label::GetCaption()
	{
		return mCaption;
	}

	void Label::SetCaption(std::string newCaption)
	{
		mCaption = newCaption;
	}

	void Label::SetFont(GameFont* newFont)
	{
		mFont = newFont;
	}

	// DEBUG
	std::string Label::GetName()
	{
		return "Label";
	}
}
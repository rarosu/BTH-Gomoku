#include "Label.hpp"

namespace Components
{
	Label::Label(ID3D10Device* device, ComponentGroup* ownerGroup, std::string caption, RECT position, 
				 GameFont::AlignHorizontal alignHorizontal, GameFont::AlignVertical alignVertical)
		: Component(ownerGroup),
		  mFont(NULL), mCaption(caption), mTextColor(C_COLOR_TEXT), 
		  mShadowColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)), mAlignHor(alignHorizontal), mAlignVer(alignVertical)
	{
		mPositionRect = position;
		int offsetX = 2;
		int offsetY = 1;
		mShadowPosition.left = mPositionRect.left + offsetX;
		mShadowPosition.right = mPositionRect.right + offsetX;
		mShadowPosition.top = mPositionRect.top + offsetY;
		mShadowPosition.bottom = mPositionRect.bottom + offsetY;
		
		int size = mPositionRect.bottom - mPositionRect.top;
		mFont = new GameFont(device, "Segoe Print", size, false, true);
	}

	void Label::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
	}

	void Label::Draw()
	{
		mFont->WriteText(mCaption, &mShadowPosition, mShadowColor, mAlignHor, mAlignVer);
		mFont->WriteText(mCaption, &mPositionRect, mTextColor, mAlignHor, mAlignVer);
	}

	void Label::LostFocus()
	{
	}
	
	void Label::GotFocus()
	{
	}

	void Label::SetCaption(std::string newCaption)
	{
		mCaption = newCaption;
	}

	// DEBUG
	std::string Label::GetName()
	{
		return "Label";
	}
}
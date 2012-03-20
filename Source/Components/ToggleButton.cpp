#include "ToggleButton.hpp"
#include <cassert>

namespace Components
{
	ToggleButton::ToggleButton(ComponentGroup* ownerGroup, RECT position, const StringVector& stringVector)
		: Button(ownerGroup, position),
		  mValues(stringVector),
		  mTextColor(D3DXCOLOR(1.0, 1.0, 1.0, 1.0)),
		  mCurrentValue(0),
		  mFont(NULL),
		  mHasBeenToggled(false)
	{
	}

	ToggleButton::~ToggleButton() throw()
	{
		SafeDelete(mFont);
	}

	void ToggleButton::Initialize(ID3D10Device* device)
	{
		Graphics buttonGraphics;
		buttonGraphics.activeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		buttonGraphics.idleColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		buttonGraphics.hoverColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
		buttonGraphics.textureUp = new Sprite(device, sViewport, "buttonBase.png", GetWidth(), GetHeight());

		Button::Initialize(device, buttonGraphics);

		int textSize = GetHeight() / 2;
		mFont = new GameFont(device, "Jing Jing", textSize, false, true);
	}

	void ToggleButton::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(GetAndResetClickStatus())
		{
			mCurrentValue++;
			mCurrentValue %= mValues.size();

			mHasBeenToggled = true;
		}
	}

	void ToggleButton::Draw()
	{
		if(!IsVisible())
			return;

		D3DXVECTOR2 pos = GetPosition();
		RECT posRect = { pos.x, pos.y, pos.x + GetWidth(), pos.y + GetHeight() };

		Button::Draw();

		std::string caption = mValues[mCurrentValue];
		if(IsEnabled())
		{
			if(IsHovered())
				mFont->WriteText(caption, &posRect, D3DXCOLOR(1.0, 1.0, 0.0, 1.0), GameFont::Center, GameFont::Middle);
			else
				mFont->WriteText(caption, &posRect, mTextColor, GameFont::Center, GameFont::Middle);
		}
		else
			mFont->WriteText(caption, &posRect, D3DXCOLOR(0.5, 0.5, 0.5, 1.0), GameFont::Center, GameFont::Middle);
	}

	const std::string& ToggleButton::GetCaption() const
	{
		return mValues[mCurrentValue];
	}

	void ToggleButton::SetCurrentValue(int currentValue)
	{
		assert(mValues.size() > 0);

		currentValue %= mValues.size();
		mCurrentValue = currentValue;
	}

	int ToggleButton::GetCurrentValue() const
	{
		return mCurrentValue;
	}

	int ToggleButton::GetNumberOfValues() const
	{
		return mValues.size();
	}

	std::string ToggleButton::GetName()
	{
		return "ToggleButton: " + GetCaption();
	}

	bool ToggleButton::GetAndResetToggleStatus()
	{
		bool result = mHasBeenToggled;
		mHasBeenToggled = false;
		return result;
	}
}
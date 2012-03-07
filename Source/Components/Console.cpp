#include "Console.hpp"
#include <string>


namespace Components
{
	const int Console::C_NUM_VERTICES = 4;

	Console::Console(ID3D10Device* device, ComponentGroup* ownerGroup, RECT position, D3DXCOLOR bgColor, UINT size)
		: ComponentGroup(ownerGroup, "Console"),
		  mDevice(device), mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0)), mFirstShowRow(0), mBackground(NULL),
		  C_HISTORY_SIZE(size)
	{
		mPositionRect = position;
		mBackground = new Sprite(mDevice, sViewport, "whitePixel.png", GetWidth(), GetHeight());

		int textHeight = 18;
		mFont = new GameFont(mDevice, "System", textHeight, false, true);
		mStream.str("");

		mMaxNumRows = (int)((GetHeight() - textHeight) / textHeight);

		RECT inputFieldPos = mPositionRect;
		inputFieldPos.top = inputFieldPos.bottom - 20;
		mInputField = new InputField(mDevice, this, this, inputFieldPos, mFont);

		int scrollWidth = 20;
		RECT scrollbarPos = { mPositionRect.right - scrollWidth, mPositionRect.top, 
							  mPositionRect.right, mPositionRect.bottom };
		mScrollbar = new Scrollbar(this, this);
		mScrollbar->Initialize(mDevice, scrollbarPos);

		SetFocusedComponent(mInputField);
		mBGColor = C_COLOR_WINDOW_BG;
}

	Console::~Console() throw()
	{
		SafeDelete(mBackground);
	}


	// Update the console
	void Console::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(!IsVisible())
			return;

		if(currInputState.Keyboard.keyIsPressed[VK_UP] && !prevInputState.Keyboard.keyIsPressed[VK_UP])
		{
			Scroll(true);
		}
		else if(currInputState.Keyboard.keyIsPressed[VK_DOWN] && !prevInputState.Keyboard.keyIsPressed[VK_DOWN])
		{
			Scroll(false);
		}

		ComponentGroup::Update(gameTime, currInputState, prevInputState);
	}

	// Draw the console
	void Console::Draw()
	{
		if(!IsVisible())
			return;

		if(mBackground != NULL)
		{
			D3DXVECTOR2 position = D3DXVECTOR2(mPositionRect.left, mPositionRect.top);
			mBackground->Draw(position, mBGColor);
		}

		POINT position = { mPositionRect.left + 3 , mPositionRect.top };
		int start = std::max<int>(0, mFirstShowRow);
		int end = std::min<int>(mFirstShowRow + mMaxNumRows, mOutput.size());
		for(int i = start; i < end; ++i)
		{
			mFont->WriteText("> " + mOutput[i].text, position, mOutput[i].color);
			position.y += mFont->GetSize();
		}

		ComponentGroup::Draw();
	}

	// Toggle showing of the console
	void Console::Toggle()
	{
		if(IsVisible())
		{
			SetVisible(false);
			LoseFocus();
		}
		else
		{
			SetVisible(true);
			SetFocus();
		}
	}

	void Console::LostFocus()
	{
		mInputField->LostFocus();
	}

	void Console::GotFocus()
	{
		SetFocusedComponent(mInputField);
	}

	void Console::MouseButtonReleased(int index, const InputState& currentState)
	{
		ComponentGroup::MouseButtonReleased(index, currentState);

		if(IsVisible())
			if(currentState.Mouse.x > mPositionRect.left && currentState.Mouse.x < mPositionRect.right &&
			   currentState.Mouse.y > mPositionRect.top && currentState.Mouse.y < mPositionRect.bottom &&
			   !mScrollbar->IsHovered())
				SetFocus();	
	}

	void Console::Scroll(bool isUp)
	{
		if(isUp)
		{
			--mFirstShowRow;
			mFirstShowRow = Clamp<int>(mFirstShowRow, 0, mOutput.size());
		}
		else
		{
			++mFirstShowRow;
			mFirstShowRow = Clamp<int>(mFirstShowRow, 0, std::max<int>(0, mOutput.size() - mMaxNumRows));
		}
	}

	void Console::RecieveInput(std::string input)
	{
		TextLine text;
		text.text = input;
		text.color = mTextColor;

		if(mOutput.size() >= C_HISTORY_SIZE)
			mOutput.erase(mOutput.begin());

		mOutput.push_back(text);

		if(mOutput.size() > (UINT)mMaxNumRows)
			mFirstShowRow = mOutput.size() - mMaxNumRows;
	}

	void Console::SetTextColor(D3DXCOLOR newColor)
	{
		mTextColor = newColor;
	}

	// DEBUG
	//std::string Console::GetName()
	//{
	//	std::string result = "Console";

	//	if(mFocusedComponent)
	//		result += " -> " + mFocusedComponent->GetName();

	//	return result;
	//}
}
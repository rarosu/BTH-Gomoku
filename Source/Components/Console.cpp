#include "Console.hpp"
#include <string>


namespace Components
{
	const int Console::C_NUM_VERTICES = 4;

	Console::Console(ID3D10Device* device, ComponentGroup* ownerGroup, RECT position, D3DXCOLOR bgColor, UINT size)
		: ComponentGroup(ownerGroup, "Console", position),
		  mDevice(device), mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0)), mFirstShowRow(0), mBackground(NULL), mInputReceiver(NULL),
		  C_HISTORY_SIZE(size)
	{
		mBackground = new Sprite(mDevice, sViewport, "whitePixel.png", GetWidth(), GetHeight());

		int textHeight = 18;
		mFont = new GameFont(mDevice, "System", textHeight, false, true);
		mStream.str("");

		mMaxNumRows = (int)((GetHeight() - textHeight) / textHeight);

		RECT inputFieldPos = { 0, GetHeight() - 20, GetWidth(), GetHeight() };
		mInputField = new InputField(mDevice, this, this, inputFieldPos, mFont);

		int scrollWidth = 20;
		RECT scrollbarPos = { GetWidth() - scrollWidth, 0, GetWidth(), GetHeight() };

		mScrollbar = new Scrollbar(this, this, scrollbarPos);
		mScrollbar->Initialize(mDevice);

		SetFocusedComponent(mInputField);
		mBGColor = C_COLOR_WINDOW_BG;
}

	Console::~Console() throw()
	{
		SafeDelete(mBackground);
	}

	void Console::SetInputReceiver(ConsoleInputReceiver* receiver)
	{
		mInputReceiver = receiver;
	}

	const ConsoleInputReceiver* Console::GetInputReceiver() const
	{
		return mInputReceiver;
	}

	// Update the console
	void Console::Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		ComponentGroup::Refresh(gameTime, currInputState, prevInputState);

		if(currInputState.Keyboard.keyIsPressed[VK_UP] && !prevInputState.Keyboard.keyIsPressed[VK_UP])
		{
			Scroll(true);
		}
		else if(currInputState.Keyboard.keyIsPressed[VK_DOWN] && !prevInputState.Keyboard.keyIsPressed[VK_DOWN])
		{
			Scroll(false);
		}
	}

	// Draw the console
	void Console::Draw()
	{
		if(!IsVisible())
			return;

		D3DXVECTOR2 position = GetPosition();

		if(mBackground != NULL)
		{	
			mBackground->Draw(position, mBGColor);
		}

		POINT textPos = { position.x + 3 , position.y };
		int start = std::max<int>(0, mFirstShowRow);
		int end = std::min<int>(mFirstShowRow + mMaxNumRows, mOutput.size());
		for(int i = start; i < end; ++i)
		{
			mFont->WriteText("> " + mOutput[i].text, textPos, mOutput[i].color);
			textPos.y += mFont->GetSize();
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
			if(IsHovered() && !mScrollbar->IsHovered())
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

	void Console::ReceiveInput(std::string input)
	{
		TextLine text;
		text.text = input;
		text.color = mTextColor;

		if(mOutput.size() >= C_HISTORY_SIZE)
			mOutput.erase(mOutput.begin());

		mOutput.push_back(text);

		if(mOutput.size() > (UINT)mMaxNumRows)
			mFirstShowRow = mOutput.size() - mMaxNumRows;
		
		if (mInputReceiver != NULL)
			mInputReceiver->ConsoleInputEntered(this, input);
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
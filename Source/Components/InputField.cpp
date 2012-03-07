#include "InputField.hpp"

namespace Components
{
	const int InputField::C_NUM_VERTICES = 4;
	const float InputField::C_MARKER_SPEED = 500;

	InputField::InputField(ID3D10Device* device, ComponentGroup* ownerGroup, InputReceiver* receiver, 
		RECT position, GameFont* font)
		: Clickable(ownerGroup),
		  mDevice(device), mBackground(NULL), mFont(font), 
		  mReceiver(receiver), mShowMarker(true), mMSSinceBlink(0.0f), mHasFocus(false)
	{
		mPositionRect = position;
	
		mBackground = new Sprite(mDevice, sViewport, "whitePixel.png", GetWidth(), GetHeight());
	}

	InputField::~InputField() throw()
	{
		SafeDelete(mBackground);
	}

	std::string InputField::GetText() const
	{
		return mFirstString.str() + mLastString.str();
	}

	bool InputField::Empty() const
	{
		return GetText().empty();
	}

	void InputField::SetText(std::string newText)
	{
		mFirstString.str("");
		mFirstString << newText;
		mLastString.str("");
	}

	void InputField::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		Clickable::Update(gameTime, currInputState, prevInputState);

		if(mHasFocus)
		{
			mMSSinceBlink += (float)gameTime.GetTimeSinceLastTick().Milliseconds;

			if(mMSSinceBlink >= C_MARKER_SPEED)
			{
				mShowMarker = !mShowMarker;
				mMSSinceBlink -= C_MARKER_SPEED;
			}
		}
		else
			mShowMarker = false;
		
	}

	void InputField::Draw()
	{
		if(mBackground != NULL)
		{
			D3DXVECTOR2 position = D3DXVECTOR2(mPositionRect.left, mPositionRect.top);
			mBackground->Draw(position);
		}

		int offset = 10;
		POINT position = { mPositionRect.left + offset, mPositionRect.top };

		std::string marker = " ";
		if(mShowMarker)
			marker = "|";
		
		mFont->WriteText(mFirstString.str() + marker + mLastString.str(), 
						 position, D3DXCOLOR(0.0, 0.0, 0.0, 1.0));
	}

	void InputField::LostFocus()
	{
		mHasFocus = false;
	}

	void InputField::GotFocus()
	{
		mHasFocus = true;
	}

	void InputField::KeyPressed(int code, const InputState& currentState)
	{
		std::string first, letter, last;

		switch(code)
		{
			case VK_RETURN:
				if(mFirstString.str() != "" || mLastString.str() != "")
				{
					if(mReceiver)
					{
						mReceiver->RecieveInput(mFirstString.str() + mLastString.str());
						mFirstString.str("");
						mLastString.str("");
					}
				}
				break;
			case VK_BACK:
				first = mFirstString.str();
				first = first.substr(0, first.size() - 1);

				mFirstString.str("");
				mFirstString << first;
				break;
			case VK_LEFT:
				if(mFirstString.str() != "")
				{
					first = mFirstString.str();
					last = mLastString.str();
					letter = first.substr(first.size() - 1, first.size());
					first = first.substr(0, first.size() - 1);

					mFirstString.str("");
					mFirstString << first;
					mLastString.str("");
					mLastString << letter << last;
				}
				break;
			case VK_RIGHT:
				if(mLastString.str() != "")
				{
					first = mFirstString.str();
					last = mLastString.str();
					letter = last.substr(0, 1);
					last = last.substr(1, last.size());

					mFirstString.str("");
					mFirstString << first << letter;
					mLastString.str("");
					mLastString << last;
				}
				break;
		}
	}

	void InputField::KeyReleased(int code, const InputState& currentState)
	{
	}

	void InputField::CharEntered(unsigned char symbol, const InputState& currentState)
	{
		mFirstString << symbol;
	}

	void InputField::MouseEntered()
	{
	}
	
	void InputField::MouseExited()
	{
	}
	
	void InputField::MousePressed(int buttonIndex)
	{
	}
	
	void InputField::MouseReleased(int buttonIndex)
	{
	}
	
	// DEBUG
	std::string InputField::GetName()
	{
		return "InputField: " + GetText();
	}
}
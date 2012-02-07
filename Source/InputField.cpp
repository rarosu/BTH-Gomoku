#include "InputField.hpp"

InputField::InputField(InputManager* manager, InputReciever* reciever, D3DXVECTOR2 position, GameFont* font)
{
	manager->AddKeyListener(this);
	mReciever = reciever;
	mPosition = position;
	mFont = font;

	mEffect = new Effect();
}

void InputField::Draw()
{
	POINT position = { (LONG)mPosition.x , (LONG)mPosition.y };
	mFont->WriteText(mStream.str(), position, D3DXCOLOR(0.0, 0.0, 0.0, 1.0));
}

void InputField::KeyPressed(int code)
{
	if(code == VK_RETURN)
	{
		if(mStream.str() != "")
		{
			mReciever->RecieveInput(mStream.str());
			//mOutput.push_back("> " + mStream.str());
			mStream.str("");
		}
	}
	else if(code == VK_BACK)
	{
		std::string temp = mStream.str();
		temp = temp.substr(0, temp.size() - 1);

		mStream.str("");
		mStream << temp;
	}
}

void InputField::KeyReleased(int code)
{
}

void InputField::CharEntered(unsigned char symbol)
{
	mStream << symbol;
}
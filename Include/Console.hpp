#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <sstream>
#include <vector>

#include "Buffer.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"
#include "GameTime.hpp"
#include "Globals.hpp"
#include "InputField.hpp"

class Console : public InputReciever /*: public KeyListener*/
{
public:
	Console(ID3D10Device* device, RECT position, D3DXCOLOR bgColor, InputManager* manager);
	void Update(GameTime gameTime);
	void Draw();
	void Toggle();

	void RecieveInput(std::string input);
	void SetTextColor(D3DXCOLOR newColor);

	/*void KeyPressed(int code);
	void KeyReleased(int code);
	void CharEntered(unsigned char symbol);*/

private:
	struct TextLine
	{
		std::string				text;
		D3DXCOLOR				color;
	};

	ID3D10Device*				mDevice;
	Effect*						mEffect;
	Buffer*						mVertexBuffer;
	GameFont*					mFont;
	D3DXCOLOR					mTextColor;
	InputField*					mInputField;

	bool						mIsToggled;
	RECT						mPosition;
	std::stringstream			mStream;
	std::vector<TextLine>		mOutput;
	int							mMaxNumRows;

	static const int			C_NUM_VERTICES;

	void CreateBuffer();
	void CreateEffect();
};
#endif
#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <sstream>
#include <vector>

#include "Buffer.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"
#include "GameTime.hpp"
#include "InputField.hpp"

class Console : public InputReciever /*: public KeyListener*/
{
public:
	Console(ID3D10Device* device, float screenWidth, float screenHeight, D3DXCOLOR bgColor, InputManager* manager);
	void Update(GameTime gameTime);
	void Draw();
	void Toggle();

	void RecieveInput(std::string input);

	/*void KeyPressed(int code);
	void KeyReleased(int code);
	void CharEntered(unsigned char symbol);*/

private:
	ID3D10Device*				mDevice;
	Effect*						mEffect;
	Buffer*						mVertexBuffer;
	GameFont*					mFont;
	D3DXCOLOR					mTextColor;
	InputField*					mInputField;

	bool						mIsToggled;
	RECT						mBounds;
	std::stringstream			mStream;
	std::vector<std::string>	mOutput;
	int							mMaxNumRows;

	static const int			C_NUM_VERTICES;

	void CreateBuffer(float width, float height);
	void CreateEffect();
};
#endif
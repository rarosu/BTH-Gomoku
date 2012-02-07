#ifndef INPUTFIELD_HPP
#define INPUTFIELD_HPP

#include <sstream>
#include <string>
#include <D3DX10.h>
#include "Effect.hpp"
#include "GameFont.hpp"
#include "InputManager.hpp"

class InputReciever
{
public:
	virtual void RecieveInput(std::string input) = 0;
};

class InputField : public KeyListener
{
public:
	InputField(InputManager* manager, InputReciever* reciever, D3DXVECTOR2 position, GameFont* font);
	//void Update();
	void Draw();

	// Methods inherited from KeyListener
	void KeyPressed(int code);
	void KeyReleased(int code);
	void CharEntered(unsigned char symbol);

private:
	struct FieldVertex
	{

	};

	std::stringstream			mStream;
	InputReciever*				mReciever;

	D3DXVECTOR2					mPosition;
	GameFont*					mFont;
	Effect*						mEffect;
};
#endif
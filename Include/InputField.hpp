#ifndef INPUTFIELD_HPP
#define INPUTFIELD_HPP

#include <sstream>
#include <string>
#include <D3DX10.h>

#include "Buffer.hpp"
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
	InputField(ID3D10Device* device, InputManager* manager, InputReciever* reciever, 
		RECT position, GameFont* font);
	//void Update();
	void Draw();

	// Methods inherited from KeyListener
	void KeyPressed(int code);
	void KeyReleased(int code);
	void CharEntered(unsigned char symbol);

private:
	ID3D10Device*				mDevice;
	Buffer*						mBuffer;
	Effect*						mEffect;

	RECT						mPosition;
	GameFont*					mFont;
	std::stringstream			mStream;
	InputReciever*				mReciever;

	static const int			C_NUM_VERTICES;

	void CreateBuffer();
	void CreateEffect();
};
#endif
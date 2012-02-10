#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Buffer.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"

//class ButtonBackground
//{
//
//}
//
//class BgColor : public ButtonBackground
//{
//
//}

class Button
{
public:
	Button();
	
	void Initialize(ID3D10Device* device, RECT position);
	void Draw();

private:
	ID3D10Device*				mDevice;
	Buffer*						mBuffer;
	Effect*						mEffect;

	RECT						mPosition;
	GameFont*					mFont;

	void CreateBuffer();
	void CreateEffect();
};
#endif
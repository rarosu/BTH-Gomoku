#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>

#include "Buffer.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"
#include "InputManager.hpp"

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
	
	void Initialize(ID3D10Device* device, RECT position, std::string caption, 
					D3DXCOLOR color = D3DXCOLOR(0.0, 0.0, 0.0, 0.0));
	void Update(InputState currInputState);
	void Draw();

private:
	struct ButtonVertex
	{
		D3DXVECTOR2				position;
		D3DXVECTOR2				uv;
	};

	ID3D10Device*				mDevice;
	Buffer*						mBuffer;
	Effect*						mEffect;

	RECT						mPosition;
	GameFont*					mFont;
	std::string					mCaption;
	D3DXCOLOR					mTextColor;
	D3DXCOLOR					mIdleColor;
	D3DXCOLOR					mHoverColor;

	void CreateBuffer();
	void CreateEffect();
};
#endif
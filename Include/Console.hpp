#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <sstream>
#include <vector>

#include "GameTime.hpp"
#include "GameFont.hpp"
#include "Buffer.hpp"
#include "InputManager.hpp"

class Console : public KeyListener
{
public:
	Console(ID3D10Device* device, float screenWidth, float screenHeight);
	void Update(GameTime gameTime);
	void Draw();
	void Toggle();

	void KeyPressed(int code);
	void KeyReleased(int code);
	void CharEntered(unsigned char symbol);

private:
	ID3D10Device*				mDevice;
	ID3D10Effect*				mEffect;
	ID3D10EffectTechnique*		mTechnique;
	ID3D10InputLayout*			mVertexLayout;
	Buffer*						mVertexBuffer;
	GameFont*					mFont;
	D3DXCOLOR					mTextColor;

	bool						mIsToggled;
	RECT						mBounds;
	std::stringstream			mStream;
	std::vector<std::string>	mOutput;
	int							mMaxNumRows;

	static const char*		C_FILENAME;
	static const int		C_NUM_VERTICES;

	HRESULT CreateEffect();
	HRESULT CreateVertexLayout();
};
#endif
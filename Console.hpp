#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "GameTime.hpp"
#include "Buffer.hpp"

class Console
{
public:
	Console(ID3D10Device* device, float screenWidth, float screenHeight);
	void Update(GameTime gameTime);
	void Draw();

private:
	ID3D10Device*			mDevice;
	ID3D10Effect*			mEffect;
	ID3D10EffectTechnique*	mTechnique;
	ID3D10InputLayout*		mVertexLayout;
	Buffer*					mVertexBuffer;

	static const char*		C_FILENAME;
	static const int		C_NUM_VERTICES;

	HRESULT CreateEffect();
	HRESULT CreateVertexLayout();
};
#endif
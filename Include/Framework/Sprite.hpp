#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "WinInclude.hpp"
#include "Buffer.hpp"
#include "Effect.hpp"
#include "Viewport.hpp"

class Sprite
{
public:
	Sprite(ID3D10Device* device, const Viewport* viewport, const std::string& filename, float width, float height);
	~Sprite() throw();
	void Draw(const D3DXVECTOR2& position, const D3DXCOLOR& tintColor = D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

private:
	struct SpriteVertex
		{
			D3DXVECTOR2				position;
			D3DXVECTOR2				uv;
		};

	ID3D10Device*				mDevice;
	VertexBuffer*				mBuffer;
	Effect*						mEffect;
	const Viewport*				mViewport;
	float						mWidth;
	float						mHeight;

	void CreateBuffer();
	void CreateEffect();
};

#endif
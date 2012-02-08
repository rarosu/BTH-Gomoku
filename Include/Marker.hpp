#ifndef MARKER_HPP
#define MARKER_HPP

#include <D3DX10.h>
#include "Buffer.hpp"
#include "Camera.hpp"
#include "Effect.hpp"

class Marker
{
public:
	Marker(ID3D10Device* device, int size);
	void Update(const Camera& camera);
	void Draw();

private:
	ID3D10Device*				mDevice;
	Buffer*						mBuffer;
	Effect*						mEffect;

	D3DXMATRIX					mWorldMatrix;

	void CreateBuffer(int size);
	void CreateEffect();
};
#endif
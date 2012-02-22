#ifndef MARKER_HPP
#define MARKER_HPP

#include <D3DX10.h>
#include "Buffer.hpp"
#include "Effect.hpp"
#include "Camera.hpp"


class Marker
{
public:
	Marker(ID3D10Device* device, int size, D3DXVECTOR3 position, D3DXCOLOR markerColor);
	void Update(const Camera& camera);
	void Draw();

private:
	ID3D10Device*				mDevice;
	VertexBuffer*				mBuffer;
	Effect*						mEffect;

	D3DXMATRIX					mWorldMatrix;
	D3DXVECTOR3					mPosition;

	void CreateBuffer(int size);
	void CreateEffect();
	void UpdateWorldMatrix();
};
#endif
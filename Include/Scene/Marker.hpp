#ifndef MARKER_HPP
#define MARKER_HPP

#include "WinInclude.hpp"
#include "Buffer.hpp"
#include "Effect.hpp"
#include "Camera.hpp"


class Marker
{
public:
	Marker(ID3D10Device* device, int size, D3DXCOLOR markerColor);
	void Update(const Camera& camera);
	void Draw(const Camera& camera, D3DXVECTOR3 drawPosition);

private:
	ID3D10Device*				mDevice;
	VertexBuffer*				mBuffer;
	Effect*						mEffect;

	D3DXMATRIX					mWorldMatrix;

	void CreateBuffer(int size);
	void CreateEffect();
	void UpdateWorldMatrix(D3DXVECTOR3 position);
};
#endif
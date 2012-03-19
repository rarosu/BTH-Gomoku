#ifndef MARKER_HPP
#define MARKER_HPP

#include "ObjModel.hpp"

class Marker
{
public:
	enum MarkerType
	{
		Ring,
		Triangle,
		Quad,
		Cross
	};
	Marker(ID3D10Device* device, MarkerType type);
	~Marker();

	void Draw(const Camera& camera, const D3DXVECTOR3& drawPosition);

private:
	ObjModel*				mMarker;
};
#endif
//#include "WinInclude.hpp"
//#include "Buffer.hpp"
//#include "Effect.hpp"
//#include "Camera.hpp"
//
//
//class Marker
//{
//public:
//	Marker(ID3D10Device* device, int size, const D3DXCOLOR& markerColor);
//	~Marker() throw();
//
//	void Draw(const Camera& camera, const D3DXVECTOR3& drawPosition);
//private:
//	ID3D10Device*				mDevice;
//	VertexBuffer*				mBuffer;
//	Effect*						mEffect;
//
//	D3DXMATRIX					mWorldMatrix;
//
//	void CreateBuffer(int size);
//	void CreateEffect();
//	void UpdateWorldMatrix(const D3DXVECTOR3& position);
//};
//#endif
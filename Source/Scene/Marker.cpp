#include "Marker.hpp"

Marker::Marker(ID3D10Device* device, MarkerType type)
	: mMarker(NULL)
{
	switch(type)
	{
		case MarkerType::Ring:
			mMarker = new ObjModel(device, "ring.obj", D3DXCOLOR(0.0, 1.0, 0.0, 1.0));
			break;
		case MarkerType::Triangle:
			mMarker = new ObjModel(device, "triangle.obj", D3DXCOLOR(0.0, 0.0, 1.0, 1.0));
			break;
		case MarkerType::Quad:
			mMarker = new ObjModel(device, "quad.obj", D3DXCOLOR(1.0, 1.0, 0.0, 1.0));
			break;
		case MarkerType::Cross:
			mMarker = new ObjModel(device, "cross.obj", D3DXCOLOR(1.0, 0.0, 0.0, 1.0));
			break;
	}
}

Marker::~Marker() throw()
{
	SafeDelete(mMarker);
}

void Marker::Draw(const Camera& camera, const D3DXVECTOR3& drawPosition)
{
	mMarker->Draw(camera, drawPosition);
}

//
//Marker::Marker(ID3D10Device* device, int size, const D3DXCOLOR& markerColor)
//{
//	mDevice = device;
//	D3DXMatrixIdentity(&mWorldMatrix);
//	UpdateWorldMatrix(D3DXVECTOR3(0.0, 0.0, 0.0));
//
//	CreateBuffer(size);
//	CreateEffect();
//
//	mEffect->SetVariable("gColor", (D3DXVECTOR4)markerColor);
//}
//
//Marker::~Marker() throw()
//{
//	SafeDelete(mBuffer);
//	SafeDelete(mEffect);
//}
//
//void Marker::Draw(const Camera& camera, const D3DXVECTOR3& drawPosition)
//{
//	UpdateWorldMatrix(drawPosition);
//	D3DXMATRIX worldViewProjection = mWorldMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix();
//	mEffect->SetVariable("gWVP", worldViewProjection);
//
//	mBuffer->Bind();
//	for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
//	{
//		mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
//		mBuffer->Draw();
//	}
//}
//
//void Marker::CreateBuffer(int size)
//{
//	D3DXVECTOR3 vertices[4];
//
//	float halfsize = (float)size / 2;
//
//	vertices[0] = D3DXVECTOR3(-halfsize, 0, -halfsize);
//	vertices[1] = D3DXVECTOR3(halfsize, 0, -halfsize);
//	vertices[2] = D3DXVECTOR3(-halfsize, 0, halfsize);
//	vertices[3] = D3DXVECTOR3(halfsize, 0, halfsize);
//
//	mBuffer = new VertexBuffer(mDevice);
//	VertexBuffer::Data bufferDesc;
//
//	bufferDesc.mUsage =					Usage::Default;
//	bufferDesc.mTopology =				Topology::TriangleStrip;
//	bufferDesc.mElementCount =			4;
//	bufferDesc.mElementSize =			sizeof(D3DXVECTOR3);
//	bufferDesc.mFirstElementPointer =	vertices;
//
//	mBuffer->SetData(bufferDesc, NULL);
//}
//
//void Marker::CreateEffect()
//{
//	mEffect = new Effect(mDevice, "Resources/Effects/Marker.fx");
//	
//	InputLayoutVector inputLayout;
//	inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT));
//
//	mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
//}
//
//void Marker::UpdateWorldMatrix(const D3DXVECTOR3& position)
//{
//	// Update rotation in matrix
//	/*float cosA = std::cos(mRotation);
//	float sinA = std::sin(mRotation);
//
//	mWorldMatrix.m[0][0] = cosA;
//	mWorldMatrix.m[0][2] = -sinA;
//	mWorldMatrix.m[2][0] = sinA;
//	mWorldMatrix.m[2][2] = cosA;*/
//
//	// Update position in matrix
//	mWorldMatrix.m[3][0] = position.x;
//	mWorldMatrix.m[3][1] = position.y;
//	mWorldMatrix.m[3][2] = position.z;
//}
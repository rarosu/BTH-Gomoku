#include "Marker.hpp"

Marker::Marker(ID3D10Device* device, int size, D3DXVECTOR3 position, D3DXCOLOR markerColor)
{
	mDevice = device;
	mPosition = position;
	D3DXMatrixIdentity(&mWorldMatrix);
	UpdateWorldMatrix();

	CreateBuffer(size);
	CreateEffect();

	mEffect->SetVariable("gColor", &(D3DXVECTOR4)markerColor);
}

void Marker::Update(const Camera& camera)
{
	UpdateWorldMatrix();
	D3DXMATRIX viewProjection = mWorldMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	mEffect->SetVariable("gVP", &viewProjection);
}

void Marker::Draw()
{
	mBuffer->Bind();
	for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
	{
		mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
		mBuffer->Draw();
	}
}

void Marker::CreateBuffer(int size)
{
	D3DXVECTOR3 vertices[4];

	float halfsize = (float)size / 2;

	vertices[0] = D3DXVECTOR3(-halfsize, 0, -halfsize);
	vertices[1] = D3DXVECTOR3(halfsize, 0, -halfsize);
	vertices[2] = D3DXVECTOR3(-halfsize, 0, halfsize);
	vertices[3] = D3DXVECTOR3(halfsize, 0, halfsize);

	mBuffer = new VertexBuffer(mDevice);
	VertexBuffer::Data bufferDesc;

	bufferDesc.mUsage =					Usage::Default;
	bufferDesc.mTopology =				Topology::TriangleStrip;
	bufferDesc.mElementCount =			4;
	bufferDesc.mElementSize =			sizeof(D3DXVECTOR3);
	bufferDesc.mFirstElementPointer =	vertices;

	mBuffer->SetData(bufferDesc, NULL);
}

void Marker::CreateEffect()
{
	mEffect = new Effect(mDevice, "Resource/Effects/Marker.fx");
	
	InputLayoutVector inputLayout;
	inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT));

	mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
}


void Marker::UpdateWorldMatrix()
{
	// Update rotation in matrix
	/*float cosA = std::cos(mRotation);
	float sinA = std::sin(mRotation);

	mWorldMatrix.m[0][0] = cosA;
	mWorldMatrix.m[0][2] = -sinA;
	mWorldMatrix.m[2][0] = sinA;
	mWorldMatrix.m[2][2] = cosA;*/

	// Update position in matrix
	mWorldMatrix.m[3][0] = mPosition.x;
	mWorldMatrix.m[3][1] = mPosition.y;
	mWorldMatrix.m[3][2] = mPosition.z;
}
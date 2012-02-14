#include "Marker.hpp"

Marker::Marker(ID3D10Device* device, int size, D3DXVECTOR3 position, D3DXCOLOR markerColor)
{
	mDevice = device;
	mPosition = position;
	D3DXMatrixIdentity(&mWorldMatrix);
	UpdateWorldMatrix();

	CreateBuffer(size);
	CreateEffect();

	mEffect->SetVectorVariable("gColor", &(D3DXVECTOR4)markerColor);
}

void Marker::Update(const Camera& camera)
{
	UpdateWorldMatrix();
	D3DXMATRIX viewProjection = mWorldMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	mEffect->SetMatrixVariable("gVP", &viewProjection);
}

void Marker::Draw()
{
	mBuffer->MakeActive();
	mEffect->MakeActive();

	for(UINT p = 0; p < mEffect->GetNumberOfPasses(); ++p)
	{
		mEffect->ApplyTechniquePass(p);
		mDevice->Draw(mBuffer->GetNumberOfElements(), 0);
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

	mBuffer = new Buffer();
	BufferInformation bufferDesc;

	bufferDesc.type =					VertexBuffer;
	bufferDesc.usage =					Buffer_Default;
	bufferDesc.numberOfElements =		4;
	bufferDesc.firstElementPointer =	vertices;
	bufferDesc.elementSize =			sizeof(D3DXVECTOR3);
	bufferDesc.topology =				D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	mBuffer->Initialize(mDevice, bufferDesc);
}

void Marker::CreateEffect()
{
	// Create an array describing each of the elements of the vertex that are inputs to the vertex shader.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] = 
	{
		{ "POSITION",					// Semantic name, must be same as the vertex shader input semantic name
		  0,							// Semantic index, if one semantic name exists for more than one element
		  DXGI_FORMAT_R32G32B32_FLOAT,	// Format of the element, R32G32B32_FLOAT is a 32-bit 3D float vector
		  0,							// Input slot, of the 0-15 slots, through wich to send vertex data
		  0,							// AlignedByteOffset, bytes from start of the vertex to this component
		  D3D10_INPUT_PER_VERTEX_DATA,	// Input data class for this input slot
		  0 } 							// 0 when slot input data class is D3D10_INPUT_PER_VERTEX_DATA
	};

	mEffect = new Effect();
	mEffect->Initialize(mDevice, "Resources/Effects/Marker.fx", vertexDesc,
		sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
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
#include "Marker.hpp"

Marker::Marker(ID3D10Device* device, int size)
{
	mDevice = device;
	D3DXMatrixIdentity(&mWorldMatrix);

	CreateBuffer(size);
	CreateEffect();

	//mEffect->SetVectorVariable("gColor", &D3DXVECTOR4(0.0, 0.0, 0.0, 1.0));
}

void Marker::Update(const Camera& camera)
{
	D3DXMATRIX viewProjection = camera.GetViewMatrix() * camera.GetProjectionMatrix();
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

	float halfsize = 20; //(float)size / 2;

	/*vertices[0] = D3DXVECTOR3(-halfsize, 3, -halfsize);
	vertices[1] = D3DXVECTOR3(halfsize, 3, -halfsize);
	vertices[2] = D3DXVECTOR3(-halfsize, 3, halfsize);
	vertices[3] = D3DXVECTOR3(halfsize, 3, halfsize);*/

	vertices[0] = D3DXVECTOR3(0, 3, 0);
	vertices[1] = D3DXVECTOR3(10, 3, 0);
	vertices[2] = D3DXVECTOR3(0, 3, 10);
	vertices[3] = D3DXVECTOR3(10, 3, 10);

	mBuffer = new Buffer();
	BufferInformation bufferDesc;

	bufferDesc.type = VertexBuffer;
	bufferDesc.usage = Buffer_Default;
	bufferDesc.numberOfElements = 4;
	bufferDesc.firstElementPointer = vertices;
	bufferDesc.elementSize = sizeof(D3DXVECTOR3);

	mBuffer->Initialize(mDevice, bufferDesc);
}

void Marker::CreateEffect()
{
	// Create an array describing each of the elements of the vertex that are inputs to the vertex shader.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] = 
	{
		{ "POSITION",					// Semantic name, must be same as the vertex shader input semantic name
		  0,							// Semantic index, if one semantic name exists for more than one element
		  DXGI_FORMAT_R32G32_FLOAT,		// Format of the element, R32G32_FLOAT is a 32-bit 2D float vector
		  0,							// Input slot, of the 0-15 slots, through wich to send vertex data
		  0,							// AlignedByteOffset, bytes from start of the vertex to this component
		  D3D10_INPUT_PER_VERTEX_DATA,	// Input data class for this input slot
		  0 } 							// 0 when slot input data class is D3D10_INPUT_PER_VERTEX_DATA
	};

	mEffect = new Effect();
	mEffect->Initialize(mDevice, "Effects/Marker.fx", D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertexDesc,
		sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
}
#include "Button.hpp"

Button::Button()
	: mDevice(NULL), mBuffer(NULL), mEffect(NULL), mFont (NULL)
{
}

void Button::Initialize(ID3D10Device* device, RECT position)
{
	mDevice = device;
	mPosition = position;

	CreateBuffer();
	CreateEffect();
}

void Button::CreateBuffer()
{
	const int numVertices = 4;
	D3DXVECTOR2 vertices[numVertices];

	vertices[0]	= TransformToViewport(D3DXVECTOR2((float)mPosition.left, (float)mPosition.top));
	vertices[1]	= TransformToViewport(D3DXVECTOR2((float)mPosition.right, (float)mPosition.top));
	vertices[2]	= TransformToViewport(D3DXVECTOR2((float)mPosition.left, (float)mPosition.bottom));
	vertices[3]	= TransformToViewport(D3DXVECTOR2((float)mPosition.right, (float)mPosition.bottom));

	mBuffer = new Buffer();
	BufferInformation bufferDesc;

	bufferDesc.type =					VertexBuffer;
	bufferDesc.usage =					Buffer_Default;
	bufferDesc.numberOfElements =		numVertices;
	bufferDesc.firstElementPointer =	vertices;
	bufferDesc.elementSize =			sizeof(D3DXVECTOR2);
	bufferDesc.topology =				D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	mBuffer->Initialize(mDevice, bufferDesc);
}

void Button::CreateEffect()
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
	mEffect->Initialize(mDevice, "Effects/Console.fx", vertexDesc,
		sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
}

void Button::Draw()
{
	mBuffer->MakeActive();
	mEffect->MakeActive();

	for(UINT p = 0; p < mEffect->GetNumberOfPasses(); ++p)
	{
		mEffect->ApplyTechniquePass(p);
		mDevice->Draw(mBuffer->GetNumberOfElements(), 0);
	}
}
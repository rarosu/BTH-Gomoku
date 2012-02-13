#include "Scene.hpp"

Scene::Scene(ID3D10Device* device) :
	mDevice(device)
{
	CreateBuffer();
	CreateEffect();

	mEffect->SetIntVariable("gWidth", 10);
	mEffect->SetFloatVariable("gInterval", 0.1f);
}

Scene::~Scene() 
{
	SafeDelete(mVertexBuffer);
}

void Scene::CreateBuffer()
{
	GridVertex vertices[4];

	vertices[0].position = D3DXVECTOR3(-1000, 0, -1000);
	vertices[0].color = D3DXCOLOR(1.0, 1.0, 0.0, 1.0);

	vertices[1].position = D3DXVECTOR3(-1000, 0, 1000);
	vertices[1].color = D3DXCOLOR(1.0, 0.0, 0.0, 1.0);

	vertices[2].position = D3DXVECTOR3(1000, 0, -1000);
	vertices[2].color = D3DXCOLOR(0.0, 1.0, 0.0, 1.0);

	vertices[3].position = D3DXVECTOR3(1000, 0, 1000);
	vertices[3].color = D3DXCOLOR(0.0, 0.0, 1.0, 1.0);

	BufferInformation bufferDesc;
	bufferDesc.elementSize				= sizeof(GridVertex);
	bufferDesc.numberOfElements			= 4;
	bufferDesc.firstElementPointer		= vertices;
	bufferDesc.type						= VertexBuffer;
	bufferDesc.usage					= Buffer_Default;
	bufferDesc.topology =				D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	mVertexBuffer = new Buffer();
	mVertexBuffer->Initialize(mDevice, bufferDesc);
}

void Scene::CreateEffect()
{
	// Create an array describing each of the elements of the vertex that are inputs to the vertex shader.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] = 
	{
		{ "POSITION",					// Semantic name, must be same as the vertex shader input semantic name
		  0,							// Semantic index, if one semantic name exists for more than one element
		  DXGI_FORMAT_R32G32B32_FLOAT,	// Format of the element, R32G32_FLOAT is a 32-bit 2D float vector
		  0,							// Input slot, of the 0-15 slots, through wich to send vertex data
		  0,							// AlignedByteOffset, bytes from start of the vertex to this component
		  D3D10_INPUT_PER_VERTEX_DATA,	// Input data class for this input slot
		  0 },							// 0 when slot input data class is D3D10_INPUT_PER_VERTEX_DATA
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(D3DXVECTOR3), D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	mEffect = new Effect();
	mEffect->Initialize(mDevice, "Resources/Effects/Grid.fx", vertexDesc,
		sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
}

void Scene::Update(const Logic::Grid* grid, const Camera& camera)
{
	D3DXMATRIX viewProjection = camera.GetViewMatrix() * camera.GetProjectionMatrix();
	mEffect->SetMatrixVariable("gVP", &viewProjection);
}

void Scene::Draw()
{
	mVertexBuffer->MakeActive();

	mEffect->MakeActive();

	for(UINT p = 0; p < mEffect->GetNumberOfPasses(); ++p)
	{
		mEffect->ApplyTechniquePass(p);
		mDevice->Draw(mVertexBuffer->GetNumberOfElements(), 0);
	}
}

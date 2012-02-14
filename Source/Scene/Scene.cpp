#include "Scene.hpp"
#include <sstream>

Scene::Scene(ID3D10Device* device, InputSubscription* inputManager) :
	mDevice(device),
	mInputManager(inputManager),
	mVertexBuffer(NULL),
	mEffect(NULL)
{
	CreateBuffer();
	CreateEffect();

	mEffect->SetIntVariable("gWidth", 10);
	mEffect->SetFloatVariable("gInterval", 0.1f);

	mInputManager->AddMouseListener(this);
}

Scene::~Scene() 
{
	mInputManager->RemoveMouseListener(this);

	SafeDelete(mVertexBuffer);
	SafeDelete(mEffect);
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
	mEffect->Initialize(mDevice, "Effects/Grid.fx", D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertexDesc,
		sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
}

D3DXVECTOR2 Scene::PickCell(const InputState& currentState)
{
	D3DXVECTOR2 normalizedMouseCoordinates = TransformToViewport(D3DXVECTOR2(currentState.Mouse.x, currentState.Mouse.y));
	D3DXVECTOR3 v;
	
	v.x = normalizedMouseCoordinates.x;
	v.y = normalizedMouseCoordinates.y;
	v.z = 1.0f;

	v.x /= mDebugRayInput.mCamera->GetProjectionMatrix()._11;
	v.y /= mDebugRayInput.mCamera->GetProjectionMatrix()._22;

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, NULL, &mDebugRayInput.mCamera->GetViewMatrix());

	D3DXVECTOR4 origin(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVECTOR4 direction(v.x, v.y, 1.0f, 0.0f);

	D3DXVec4Transform(&origin, &origin,  &viewInverse);
	D3DXVec4Transform(&direction, &direction, &viewInverse);

	// We want to know when the ray hits the XZ-plane, thus we want to know when
	// origin.y - t * direction.y = 0. Which gives the following solution:
	float t = -origin.y / direction.y;

	D3DXVECTOR3 hit = origin + direction * t;
	//mDebugRayInput.mMarker->mPosition = D3DXVECTOR3(hit.x, hit.y, hit.z);
	
	D3DXVECTOR2 cell = D3DXVECTOR2(((int)hit.x / 10), ((int)hit.z / 10));

	return cell;
}

void Scene::Update(const Logic::Grid* grid, const Camera& camera, const InputState& currentInput, DebugRayInput debugRayInput)
{
	mDebugRayInput = debugRayInput;

	D3DXMATRIX viewProjection = camera.GetViewMatrix() * camera.GetProjectionMatrix();
	mEffect->SetMatrixVariable("gVP", &viewProjection);
	D3DXVECTOR2 cell = PickCell(currentInput);

	if (currentInput.Mouse.buttonIsPressed[C_MOUSE_LEFT])
	{
		std::stringstream stream;
		stream << "Cell: (" << cell.x << ", " << cell.y << ")";
		mDebugRayInput.mConsole->RecieveInput(stream.str());
		stream.str("");
	}

	mEffect->SetVectorVariable("gMarkedCell", &D3DXVECTOR4(cell.x, cell.y, 0.0, 0.0));
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

void Scene::MouseButtonPressed(int index, const InputState& currentState)
{
	
}

void Scene::MouseButtonReleased(int index, const InputState& currentState)
{

}

void Scene::MouseWheelMoved(short delta, const InputState& currentState) {}

RECT Scene::GetVisibleGrid(const Camera& camera) const
{

}
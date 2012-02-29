#include "Scene.hpp"
#include <sstream>

Scene::Scene(ID3D10Device* device, InputSubscription* inputManager) :
	mDevice(device),
	mInputSubscription(inputManager),
	mVertexBuffer(NULL),
	mEffect(NULL)
{
	CreateBuffer();
	CreateEffect();

	mEffect->SetVariable("gWidth", 10);
	mEffect->SetVariable("gInterval", 0.1f);
}

Scene::~Scene() throw()
{
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

	mVertexBuffer = new VertexBuffer(mDevice);

	VertexBuffer::Data bufferDesc;
	bufferDesc.mUsage =					Usage::Default;
	bufferDesc.mTopology =				Topology::TriangleStrip;
	bufferDesc.mElementCount =			4;
	bufferDesc.mElementSize	=			sizeof(GridVertex);
	bufferDesc.mFirstElementPointer	=	vertices;
	
	mVertexBuffer->SetData(bufferDesc, NULL);
}

void Scene::CreateEffect()
{
	mEffect = new Effect(mDevice, "Resources/Effects/Grid.fx");

	InputLayoutVector inputLayout;
	inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT));
	inputLayout.push_back(InputLayoutElement("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT));

	mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
}

void Scene::Update(const Logic::Grid& grid, const Camera& camera, const Viewport& viewport, const InputState& currentInput)
{
	// Pick cell
	D3DXVECTOR2 cell = PickCell(viewport, currentInput.Mouse.x, currentInput.Mouse.y, camera);
	mEffect->SetVariable("gMarkedCell", D3DXVECTOR4(cell.x, cell.y, 0.0, 0.0));
}

void Scene::Draw(const Camera& camera)
{
	// Create View-Projection-matrix
	D3DXMATRIX viewProjection = camera.GetViewMatrix() * camera.GetProjectionMatrix();
	mEffect->SetVariable("gVP", viewProjection);

	// Render the buffer
	mVertexBuffer->Bind();
	for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
	{
		mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
		mVertexBuffer->Draw();
	}
}

void Scene::MouseButtonPressed(int index, const InputState& currentState) {}
void Scene::MouseButtonReleased(int index, const InputState& currentState) {}
void Scene::MouseWheelMoved(short delta, const InputState& currentState) {}

D3DXVECTOR2 Scene::PickCell(const Viewport& viewport, int mouseX, int mouseY, const Camera& camera) const
{
	D3DXVECTOR2 normalizedMouseCoordinates = viewport.TransformToViewport(D3DXVECTOR2((float)mouseX, (float)mouseY));
	D3DXVECTOR3 v;
	
	v.x = normalizedMouseCoordinates.x;
	v.y = normalizedMouseCoordinates.y;
	v.z = 1.0f;

	v.x /= camera.GetProjectionMatrix()._11;
	v.y /= camera.GetProjectionMatrix()._22;

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse, NULL, &camera.GetViewMatrix());

	D3DXVECTOR4 origin(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVECTOR4 direction(v.x, v.y, 1.0f, 0.0f);

	D3DXVec4Transform(&origin, &origin,  &viewInverse);
	D3DXVec4Transform(&direction, &direction, &viewInverse);

	// We want to know when the ray hits the XZ-plane, thus we want to know when
	// origin.y - t * direction.y = 0. Which gives the following solution:
	float t = -origin.y / direction.y;

	D3DXVECTOR3 hit = origin + direction * t;
	D3DXVECTOR2 cell = D3DXVECTOR2(float((int)hit.x / 10), float((int)hit.z / 10));

	return cell;
}

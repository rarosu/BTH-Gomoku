#include "Scene.hpp"
#include <sstream>

const int Scene::C_GRID_WIDTH = 32;
const int Scene::C_GRID_HEIGHT = 32;
const int Scene::C_CELL_SIZE = 32;
const float Scene::C_BORDER_SIZE = 0.2f;

Scene::Scene(ID3D10Device* device) :
	mDevice(device),
	mVertexBuffer(NULL),
	mEffect(NULL)
{
	CreateBuffer();
	CreateEffect();

	mEffect->SetVariable("gWidth", C_CELL_SIZE);
	mEffect->SetVariable("gInterval", C_BORDER_SIZE * 0.5f);
	mEffect->SetVariable("gGridColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
}

Scene::~Scene() throw()
{
	SafeDelete(mVertexBuffer);
	SafeDelete(mEffect);
}

void Scene::CreateBuffer()
{
	const int C_VERTEX_COUNT_X = C_GRID_WIDTH + 1;
	const int C_VERTEX_COUNT_Z = C_GRID_HEIGHT + 1;
	const int C_VERTEX_COUNT = C_VERTEX_COUNT_X * C_VERTEX_COUNT_Z;
	GridVertex vertices[C_VERTEX_COUNT];

	for (int z = 0; z < C_VERTEX_COUNT_Z; ++z)
	{
		for (int x = 0; x < C_VERTEX_COUNT_X; ++x)
		{
			vertices[z * C_VERTEX_COUNT_Z + x].position = D3DXVECTOR3(x * C_CELL_SIZE, 0.0f, z * C_CELL_SIZE);
		}
	}

	for (int z = 0; z < C_VERTEX_COUNT_Z; ++z)
	{
		for (int x = 0; x < C_VERTEX_COUNT_X; ++x)
		{

		}
	}

	mVertexBuffer = new VertexBuffer(mDevice);

	VertexBuffer::Data bufferDesc;
	bufferDesc.mUsage =					Usage::Default;
	bufferDesc.mTopology =				Topology::TriangleStripAdjancency;
	bufferDesc.mElementCount =			C_VERTEX_COUNT;
	bufferDesc.mElementSize	=			sizeof(GridVertex);
	bufferDesc.mFirstElementPointer	=	vertices;
	
	mVertexBuffer->SetData(bufferDesc, NULL);
}

void Scene::CreateEffect()
{
	mEffect = new Effect(mDevice, "Resources/Effects/Grid.fx");

	InputLayoutVector inputLayout;
	inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT));

	mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
}

void Scene::Update(const Logic::Grid& grid, const Camera& camera, const Viewport& viewport, const InputState& currentInput)
{
	// Pick cell
	mHoveredCell = PickCell(viewport, currentInput.Mouse.x, currentInput.Mouse.y, camera);
}

void Scene::Draw(const Camera& camera)
{
	// Create View-Projection-matrix
	D3DXMATRIX viewProjection = camera.GetViewMatrix() * camera.GetProjectionMatrix();
	mEffect->SetVariable("gVP", viewProjection);
	mEffect->SetVariable("gMarkedCell", D3DXVECTOR4(mHoveredCell.x, mHoveredCell.y, 0.0, 0.0));

	// Render the buffer
	mVertexBuffer->Bind();
	for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
	{
		mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
		mVertexBuffer->Draw();
	}
}

Logic::Cell Scene::PickCell(const Viewport& viewport, int mouseX, int mouseY, const Camera& camera) const
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

	// TODO: Fix this description
	// We want to know when the ray hits the XZ-plane, thus we want to know when
	// origin.y - t * direction.y = 0. Which gives the following solution:
	float t = -origin.y / direction.y;

	D3DXVECTOR3 hit = origin + direction * t;
	Logic::Cell cell = Logic::Cell(static_cast<int>(hit.x) / 10, static_cast<int>(hit.z) / 10);

	return cell;
}

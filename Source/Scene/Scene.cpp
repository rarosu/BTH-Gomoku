#include "Scene.hpp"
#include <sstream>

const int Scene::C_GRID_WIDTH = 64;
const int Scene::C_GRID_HEIGHT = 64;
const int Scene::C_CELL_SIZE = 32;
const float Scene::C_BORDER_SIZE = 0.2f;

Scene::Scene(ID3D10Device* device, Components::ComponentGroup* ownerGroup, float aspectRatio, const Logic::Grid* grid, unsigned int playerCount) 
	: Component(ownerGroup, RECT()),
	  mDevice(device),
	  mVertexBuffer(NULL),
	  mEffect(NULL),
	  mCellTexture(NULL),
	  mCamera(NULL),
	  mGrid(grid)
{
	// Create vertex buffer and compile effect
	CreateBuffer();
	CreateEffect();

	// Load resources
	if (FAILED(D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/cell.png", NULL, NULL, &mCellTexture, NULL)))
		throw std::ios::failure("Failed to load texture: cell.png");

	// Set effect constants
	mEffect->SetVariable("gWidth", C_CELL_SIZE);
	mEffect->SetVariable("gInterval", C_BORDER_SIZE * 0.5f);
	mEffect->SetVariable("gGridColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	mEffect->SetVariable("gCellTexture", mCellTexture);

	// Setup model matrix
	D3DXVECTOR2 translation;
	translation.x = -C_GRID_WIDTH * C_CELL_SIZE * 0.5f;
	translation.y = -C_GRID_HEIGHT * C_CELL_SIZE * 0.5f;
	translation.x -= C_CELL_SIZE * 0.5f;
	translation.y -= C_CELL_SIZE * 0.5f;

	D3DXMatrixTranslation(&mModelMatrix, translation.x, 0.0f, translation.y);

	// Setup camera
	mFrustum.nearDistance = 1.0f;
	mFrustum.farDistance = 1000.0f;
	mFrustum.fovY = D3DX_PI * 0.25f;
	mFrustum.aspectRatio = aspectRatio;

	mCamera = new Camera(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), mFrustum);
	
	// TODO: Setup markers PROPERLY
	// Reminder: Markers do not have a proper copy constructor, thus vector must contain pointers
	for (unsigned int i = 0; i < playerCount; ++i)
	{
		mMarkers.push_back(new Marker(mDevice, C_CELL_SIZE * 0.5f, D3DXCOLOR(0.05 * i, 0.1 * i, 0.2 * i, 1.0f)));
	}
}

Scene::~Scene() throw()
{
	for (unsigned int i = 0; i < mMarkers.size(); ++i)
	{
		delete mMarkers[i];
	}

	SafeDelete(mVertexBuffer);
	SafeDelete(mEffect);
	SafeRelease(mCellTexture);
	SafeDelete(mCamera);
}

void Scene::CreateBuffer()
{
	const int C_VERTEX_COUNT = C_GRID_HEIGHT * C_GRID_WIDTH * 6;

	int k = 0;
	GridVertex* vertices = new GridVertex[C_VERTEX_COUNT];
	for (int z = 0; z < C_GRID_HEIGHT; ++z)
	{
		for (int x = 0; x < C_GRID_WIDTH; ++x)
		{
			CellFace face;
			face.mVertices[0].position = D3DXVECTOR3(x * C_CELL_SIZE, 0.0f, z * C_CELL_SIZE);
			face.mVertices[0].uv = D3DXVECTOR2(0, 0);
			face.mVertices[1].position = D3DXVECTOR3(x * C_CELL_SIZE, 0.0f, (z + 1) * C_CELL_SIZE);
			face.mVertices[1].uv = D3DXVECTOR2(0, 1);
			face.mVertices[2].position = D3DXVECTOR3((x + 1) * C_CELL_SIZE, 0.0f, z * C_CELL_SIZE);
			face.mVertices[2].uv = D3DXVECTOR2(1, 0);
			face.mVertices[3].position = D3DXVECTOR3(x * C_CELL_SIZE, 0.0f, (z + 1) * C_CELL_SIZE);
			face.mVertices[3].uv = D3DXVECTOR2(0, 1);
			face.mVertices[4].position = D3DXVECTOR3((x + 1) * C_CELL_SIZE, 0.0f, z * C_CELL_SIZE);
			face.mVertices[4].uv = D3DXVECTOR2(1, 0);
			face.mVertices[5].position = D3DXVECTOR3((x + 1) * C_CELL_SIZE, 0.0f, (z + 1) * C_CELL_SIZE);
			face.mVertices[5].uv = D3DXVECTOR2(1, 1);

			vertices[k] = face.mVertices[0];
			vertices[k + 1] = face.mVertices[1];
			vertices[k + 2] = face.mVertices[2];
			vertices[k + 3] = face.mVertices[3];
			vertices[k + 4] = face.mVertices[4];
			vertices[k + 5] = face.mVertices[5];

			k += 6;
		}
	}

	mVertexBuffer = new VertexBuffer(mDevice);

	VertexBuffer::Data bufferDesc;
	bufferDesc.mUsage				= Usage::Default;
	bufferDesc.mTopology			= Topology::TriangleList;
	bufferDesc.mElementCount		= C_VERTEX_COUNT;
	bufferDesc.mElementSize			= sizeof(GridVertex);
	bufferDesc.mFirstElementPointer	= vertices;
	
	mVertexBuffer->SetData(bufferDesc, NULL);

	SafeDelete(vertices);
}

void Scene::CreateEffect()
{
	mEffect = new Effect(mDevice, "Resources/Effects/Grid.fx");

	InputLayoutVector inputLayout;
	inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT));
	inputLayout.push_back(InputLayoutElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT));

	mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
}

void Scene::HandleKeyPress(const InputState& currentInput, const GameTime& gameTime)
{
	// Check for movement of the camera.
	if(currentInput.Keyboard.keyIsPressed['Q'])
		mCamera->MoveLeft(gameTime);
	else if(currentInput.Keyboard.keyIsPressed['E'])
		mCamera->MoveRight(gameTime);

	if(currentInput.Keyboard.keyIsPressed['W'])
		mCamera->MoveForward(gameTime);
	else if(currentInput.Keyboard.keyIsPressed['S'])
		mCamera->MoveBack(gameTime);

	if(currentInput.Keyboard.keyIsPressed['A'])
		mCamera->TurnHorizontal(gameTime, true);
	else if(currentInput.Keyboard.keyIsPressed['D'])
		mCamera->TurnHorizontal(gameTime, false);
}

void Scene::Update(const Logic::Grid& grid, const InputState& currentInput, const InputState& previousInput, const GameTime& gameTime)
{
	mCamera->Update(previousInput, currentInput, gameTime);

	// Pick cell
	mHoveredCell = PickCell(currentInput.Mouse.x, currentInput.Mouse.y);

	if(!HasFocus())
		return;

	HandleKeyPress(currentInput, gameTime);
}

void Scene::Draw()
{
	// Create Model-View-Projection-matrix
	D3DXMATRIX modelViewProjection = mModelMatrix * mCamera->GetViewMatrix() * mCamera->GetProjectionMatrix();
	
	int c = C_CELL_SIZE / 2;
	float left = mHoveredCell.x * C_CELL_SIZE - c;
	float up = mHoveredCell.y * C_CELL_SIZE + c;
	float right = mHoveredCell.x * C_CELL_SIZE + c;
	float down = mHoveredCell.y * C_CELL_SIZE - c;

	mEffect->SetVariable("gLeft", left);
	mEffect->SetVariable("gUp", up);
	mEffect->SetVariable("gRight", right);
	mEffect->SetVariable("gDown", down);

	mEffect->SetVariable("gModel", mModelMatrix);
	mEffect->SetVariable("gMVP", modelViewProjection);
	mEffect->SetVariable("gMarkedCell", D3DXVECTOR2(mHoveredCell.x, mHoveredCell.y));

	// Render the grid
	mVertexBuffer->Bind();
	for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
	{
		mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
		mVertexBuffer->Draw();
	}

	// Render the markers
	for (Logic::Grid::MarkerMap::const_iterator it = mGrid->GetMarkerMapStart(); it != mGrid->GetMarkerMapEnd(); it++)
	{
		mMarkers[it->second]->Draw(*mCamera, D3DXVECTOR3(it->first.x * C_CELL_SIZE, 1.0f, it->first.y * C_CELL_SIZE));
	}
}

void Scene::LostFocus()
{

}

void Scene::GotFocus()
{
}

std::string Scene::GetName()
{
	return "Scene";
}

void Scene::KeyPressed(int code, const InputState& currentState)
{
}

void Scene::KeyReleased(int code, const InputState& currentState)
{
}

void Scene::MouseWheelMoved(short delta, const InputState& currentState)
{
	mCamera->Zoom(delta);
}

void Scene::ResizeFrustum(float aspectRatio)
{
	mFrustum.aspectRatio = aspectRatio;
	mCamera->CreateProjectionMatrix(mFrustum);
}

float FindClosestMultiple(float x, float interval)
{
	float a = std::floor(x / interval) * interval;
	float b = (x - a) / interval;

	if (b <= 0.5f)
		return a;
	else
		return a + interval;
}

Logic::Cell Scene::PickCell(int mouseX, int mouseY) const
{
	D3DXVECTOR2 screenSpaceCoordinates = sViewport->TransformToViewport(D3DXVECTOR2((float)mouseX, (float)mouseY));
	D3DXMATRIX inverseProjection = mCamera->GetProjectionMatrix();
	D3DXMATRIX inverseView = mCamera->GetViewMatrix();

	D3DXMatrixInverse(&inverseProjection, NULL, &inverseProjection);
	D3DXMatrixInverse(&inverseView, NULL, &inverseView);

	D3DXVECTOR4 nearPos = D3DXVECTOR4(screenSpaceCoordinates.x, screenSpaceCoordinates.y, 0.0f, 1.0f);
	D3DXVECTOR4 farPos = D3DXVECTOR4(screenSpaceCoordinates.x, screenSpaceCoordinates.y, 1.0f, 1.0f);

	D3DXVec4Transform(&nearPos, &nearPos, &(inverseProjection * inverseView));
	D3DXVec4Transform(&farPos, &farPos, &(inverseProjection * inverseView));

	farPos /= farPos.w;

	D3DXVECTOR3 origin = D3DXVECTOR3(nearPos.x, nearPos.y, nearPos.z);
	D3DXVECTOR3 direction = D3DXVECTOR3(farPos.x - nearPos.x, farPos.y - nearPos.y, farPos.z - nearPos.z);
	D3DXVec3Normalize(&direction, &direction);

	float t = -origin.y / direction.y;
	D3DXVECTOR3 hit = origin + direction * t;

	float closestMultipleX = FindClosestMultiple(hit.x, C_CELL_SIZE);
	float closestMultipleZ = FindClosestMultiple(hit.z, C_CELL_SIZE);
	return Logic::Cell(static_cast<int>(closestMultipleX) / C_CELL_SIZE, static_cast<int>(closestMultipleZ) / C_CELL_SIZE);
}

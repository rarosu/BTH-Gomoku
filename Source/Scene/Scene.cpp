#include "Scene.hpp"
#include <sstream>

const int Scene::C_GRID_WIDTH = 64;
const int Scene::C_GRID_HEIGHT = 64;
const int Scene::C_CELL_SIZE = 32;
const float Scene::C_BORDER_SIZE = 0.2f;

Scene::Scene(ID3D10Device* device) :
	mDevice(device),
	mVertexBuffer(NULL),
	mEffect(NULL)
{
	CreateBuffer();
	CreateEffect();

	if (FAILED(D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/cell.png", NULL, NULL, &mCellTexture, NULL)))
		throw std::runtime_error("Failed to load texture: cell.png");

	mEffect->SetVariable("gWidth", C_CELL_SIZE);
	mEffect->SetVariable("gInterval", C_BORDER_SIZE * 0.5f);
	mEffect->SetVariable("gGridColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	mEffect->SetVariable("gCellTexture", mCellTexture);

	D3DXVECTOR2 translation;
	translation.x = -C_GRID_WIDTH * C_CELL_SIZE * 0.5f;
	translation.y = -C_GRID_HEIGHT * C_CELL_SIZE * 0.5f;
	translation.x -= C_CELL_SIZE * 0.5f;
	translation.y -= C_CELL_SIZE * 0.5f;

	D3DXMatrixTranslation(&mModelMatrix, translation.x, 0.0f, translation.y);

	mFile.open("grid.txt", std::ios::trunc);
}

Scene::~Scene() throw()
{
	SafeDelete(mVertexBuffer);
	SafeDelete(mEffect);
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
	bufferDesc.mUsage =					Usage::Default;
	bufferDesc.mTopology =				Topology::TriangleList;
	bufferDesc.mElementCount =			C_VERTEX_COUNT;
	bufferDesc.mElementSize	=			sizeof(GridVertex);
	bufferDesc.mFirstElementPointer	=	vertices;
	
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

void Scene::Update(const Logic::Grid& grid, const Camera& camera, const Viewport& viewport, const InputState& currentInput)
{
	// Pick cell
	mHoveredCell = PickCell(viewport, currentInput.Mouse.x, currentInput.Mouse.y, camera);
}

void Scene::Draw(const Camera& camera)
{
	// Create View-Projection-matrix
	D3DXMATRIX modelViewProjection = mModelMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix();
	
	int c = C_CELL_SIZE / 2;																	//TEST
	/*D3DXVECTOR4 upLeft = D3DXVECTOR4(mHoveredCell.x - c, 0.0f, mHoveredCell.y + c, 1.0f);		
	D3DXVECTOR4 upRight = D3DXVECTOR4(mHoveredCell.x + c, 0.0f, mHoveredCell.y + c, 1.0f);
	D3DXVECTOR4 downRight = D3DXVECTOR4(mHoveredCell.x + c, 0.0f, mHoveredCell.y - c, 1.0f);
	D3DXVECTOR4 downLeft = D3DXVECTOR4(mHoveredCell.x - c, 0.0f, mHoveredCell.y - c, 1.0f);	*/
	float left = mHoveredCell.x * C_CELL_SIZE - c;
	float up = mHoveredCell.y * C_CELL_SIZE + c;
	float right = mHoveredCell.x * C_CELL_SIZE + c;
	float down = mHoveredCell.y * C_CELL_SIZE - c;

	mFile << "(" << mHoveredCell.x << ", " << mHoveredCell.y << ")" << std::endl;
	//mFile << "(" << left << ", " << right << ") (" << down << ", " << up << ")" << std::endl;

	mEffect->SetVariable("gLeft", left);
	mEffect->SetVariable("gUp", up);
	mEffect->SetVariable("gRight", right);
	mEffect->SetVariable("gDown", down);												//END TEST

	mEffect->SetVariable("gModel", mModelMatrix);
	mEffect->SetVariable("gMVP", modelViewProjection);
	mEffect->SetVariable("gMarkedCell", D3DXVECTOR2(mHoveredCell.x, mHoveredCell.y));

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

	/*v.x /= camera.GetProjectionMatrix()._11;
	v.y /= camera.GetProjectionMatrix()._22;*/

	D3DXMATRIX viewProjInverse;
	D3DXMatrixInverse(&viewProjInverse, NULL, &(mModelMatrix * camera.GetViewMatrix() * camera.GetProjectionMatrix()));

	D3DXVECTOR4 origin(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVECTOR4 direction(v.x, v.y, 1.0f, 0.0f);

	D3DXVec4Transform(&origin, &origin,  &viewProjInverse);
	D3DXVec4Transform(&direction, &direction, &viewProjInverse);

	// TODO: Fix this description
	// We want to know when the ray hits the XZ-plane, thus we want to know when
	// origin.y - t * direction.y = 0. Which gives the following solution:
	float t = -origin.y / direction.y;

	D3DXVECTOR3 hit = origin + direction * t;
	Logic::Cell cell = Logic::Cell(static_cast<int>(hit.x) / 10, static_cast<int>(hit.z) / 10);

	return cell;
}

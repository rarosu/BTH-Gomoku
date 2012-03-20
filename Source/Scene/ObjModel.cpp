#include "ObjModel.hpp"
#include <sstream>

ObjModel::ObjModel(ID3D10Device* device, std::string filename, D3DXCOLOR color)
	: mDevice(device)
{
	mMatrixWorld = new D3DXMATRIX();
	D3DXMatrixIdentity(mMatrixWorld);
	
	if(!Load("Resources/Objects/" + filename))
		return;

	CreateBuffers();
	CreateEffect();

	/*ID3D10ShaderResourceView *pSRView = NULL;
	D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/red.png", NULL, NULL, &pSRView, NULL );
	mEffect->SetVariable("modelTexture", pSRView);*/

	mEffect->SetVariable("g_lightDirection", D3DXVECTOR4(50.0, -10.0, 50.0, 0.0));
	mEffect->SetVariable("g_modelColor", (D3DXVECTOR4)color);
}

ObjModel::~ObjModel() throw()
{

}

bool ObjModel::Load(std::string filename)
{
	std::ifstream file;
	std::vector<D3DXVECTOR3> outPositions;
	std::vector<D3DXVECTOR2> outUVCoords;
	std::vector<D3DXVECTOR3> outNormals;
	
	file.open(filename.c_str(), std::ios_base::in);
	
	if(!file.is_open())
		return false;

	while(!file.eof())
	{
		// Read first line of file.
		std::string line;
		std::getline(file, line);

		// Copy line to a stringstream and copy first word into string key
		std::stringstream streamLine;
		streamLine.str(line);
		std::string key;
		streamLine >> key;

		if(key == "v")
		{
			D3DXVECTOR3 currPos;
			streamLine >> currPos.x;
			streamLine >> currPos.y;
			streamLine >> currPos.z;
			outPositions.push_back(currPos);
		}
		else if(key == "vt")
		{
			D3DXVECTOR2 currUV;
			streamLine >> currUV.x;
			streamLine >> currUV.y;
			currUV.y = 1 - currUV.y;
			outUVCoords.push_back(currUV);
		}
		else if(key == "vn")
		{
			D3DXVECTOR3 currNormal;
			streamLine >> currNormal.x;
			streamLine >> currNormal.y;
			streamLine >> currNormal.z;
			outNormals.push_back(currNormal);
		}
		else if(key == "f")
		{
			int pos[3]; 
			int uv[3];
			int norm[3];

			for(int i = 0; i < 3; ++i)
			{
				streamLine >> pos[i];
				streamLine.ignore();
				streamLine >> uv[i];
				streamLine.ignore();
				streamLine >> norm[i];

				Vertex currVertex;
				currVertex.Position = outPositions[pos[i] - 1];
				//currVertex.UV = outUVCoords[uv[i] - 1];
				currVertex.Normal = outNormals[norm[i] - 1];
				mVertices.push_back(currVertex);
			}
		}
	}

	return true;
}

void ObjModel::UpdateWorldMatrix(D3DXVECTOR3 position)
{
	//// Update rotation in matrix
	//float cosA = std::cos(mRotation);
	//float sinA = std::sin(mRotation);

	//mMatrixWorld->m[0][0] = cosA;
	//mMatrixWorld->m[0][2] = -sinA;
	//mMatrixWorld->m[2][0] = sinA;
	//mMatrixWorld->m[2][2] = cosA;

	// Update position in matrix
	mMatrixWorld->m[3][0] = position.x;
	mMatrixWorld->m[3][1] = position.y;
	mMatrixWorld->m[3][2] = position.z;
}

void ObjModel::CreateBuffers()
{
	mVertexBuffer = new VertexBuffer(mDevice);
	
	VertexBuffer::Data vbDesc;
	vbDesc.mElementSize			= sizeof(Vertex);
	vbDesc.mElementCount		= mVertices.size();
	vbDesc.mFirstElementPointer	= &mVertices[0];
	vbDesc.mUsage				= Usage::Default;
	vbDesc.mTopology			= Topology::TriangleList;

	mVertexBuffer->SetData(vbDesc, NULL);
}

// Compile and create the shader/effect
void ObjModel::CreateEffect()
{
	mEffect = new Effect(mDevice, "Resources/Effects/ObjModel.fx");

	InputLayoutVector inputLayout;
	inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT));
	inputLayout.push_back(InputLayoutElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT));
	//inputLayout.push_back(InputLayoutElement("UV", DXGI_FORMAT_R32G32_FLOAT));

	mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
}

void ObjModel::Draw(const Camera& camera, D3DXVECTOR3 drawPosition)
{
	UpdateWorldMatrix(drawPosition);
	D3DXMATRIX worldViewProjection = (*mMatrixWorld) * (camera.GetViewMatrix() * camera.GetProjectionMatrix());
	mEffect->SetVariable("g_matWorld", *mMatrixWorld);
	mEffect->SetVariable("g_matWVP", worldViewProjection);

	mVertexBuffer->Bind();
	for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
	{
		mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
		mVertexBuffer->Draw();
	}
}
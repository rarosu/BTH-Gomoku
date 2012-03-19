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
	mEffect->SetVariable("g_modelColor", (D3DXVECTOR4)color);
}

ObjModel::~ObjModel() throw()
{

}

bool ObjModel::Load(std::string filename)
{
	std::ifstream file;
	char buffer[1024];
	std::vector<D3DXVECTOR3> outPositions;
	std::vector<D3DXVECTOR2> outUVCoords;
	std::vector<D3DXVECTOR3> outNormals;
	
	file.open(filename.c_str(), std::ios_base::binary);
	
	if(!file.is_open())
		return false;

	while(file.getline(buffer, 1024))
	{
		char key[1024];

		sscanf(buffer, "%s", key);
		if(strcmp(key, "v") == 0)
		{
			D3DXVECTOR3 currPos;
			sscanf(buffer, "v %f %f %f", &currPos.x, &currPos.y, &currPos.z);
			outPositions.push_back(currPos);
		}
		else if(strcmp(key, "vt") == 0)
		{
			D3DXVECTOR2 currUV;
			sscanf(buffer, "vt %f %f", &currUV.x, &currUV.y);
			currUV.y = 1 - currUV.y;
			outUVCoords.push_back(currUV);
		}
		else if(strcmp(key, "vn") == 0)
		{
			D3DXVECTOR3 currNormal;
			sscanf(buffer, "vn %f %f %f", &currNormal.x, &currNormal.y, &currNormal.z);
			outNormals.push_back(currNormal);
		}
		else if(strcmp(key, "f") == 0)
		{
			int pos1, pos2, pos3, uv1, uv2, uv3, norm1, norm2, norm3;
			sscanf(buffer, "f %i/%i/%i %i/%i/%i %i/%i/%i", &pos1, &uv1, &norm1, &pos2, 
				&uv2, &norm2, &pos3, &uv3, &norm3);

			Vertex currVertex;
			currVertex.Position = outPositions[pos1 - 1];
			//currVertex.UV = outUVCoords[uv1 - 1];
			currVertex.Normal = outNormals[norm1 - 1];
			mVertices.push_back(currVertex);

			currVertex.Position = outPositions[pos2 - 1];
			//currVertex.UV = outUVCoords[uv2 - 1];
			currVertex.Normal = outNormals[norm2 - 1];
			mVertices.push_back(currVertex);

			currVertex.Position = outPositions[pos3 - 1];
			//currVertex.UV = outUVCoords[uv3 - 1];
			currVertex.Normal = outNormals[norm3 - 1];
			mVertices.push_back(currVertex);
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
	mEffect->SetVariable("g_matWorld", mMatrixWorld);
	mEffect->SetVariable("g_matWVP", worldViewProjection);

	mVertexBuffer->Bind();
	for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
	{
		mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
		mVertexBuffer->Draw();
	}
}
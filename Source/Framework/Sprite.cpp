#include "Sprite.hpp"

Sprite::Sprite(ID3D10Device* device, const Viewport* viewport, const std::string& filename, float width, float height)
	: mDevice(device), mViewport(viewport), mWidth(width), mHeight(height), mBuffer(NULL), mEffect(NULL)
{
	CreateBuffer();
	CreateEffect();
	ID3D10ShaderResourceView* texture;
	std::string path = "Resources/Textures/" + filename;
	D3DX10CreateShaderResourceViewFromFile(mDevice, path.c_str(), NULL, NULL, &texture, NULL);
	mEffect->SetVariable("image", texture);
}

Sprite::~Sprite() throw()
{
	SafeDelete(mBuffer);
	SafeDelete(mEffect);
}

void Sprite::CreateBuffer()
{
	const int numVertices = 4;
	SpriteVertex vertices[numVertices];

	//vertices[0].position	= mViewport->TransformToViewport(D3DXVECTOR2(0, 0));
	vertices[0].position	= D3DXVECTOR2(0, 0);
	vertices[0].uv			= D3DXVECTOR2(0, 0);

	//vertices[1].position	= mViewport->TransformToViewport(D3DXVECTOR2(mWidth, 0));
	vertices[1].position	= D3DXVECTOR2(mWidth, 0);
	vertices[1].uv			= D3DXVECTOR2(1, 0);

	//vertices[2].position	= mViewport->TransformToViewport(D3DXVECTOR2(0, mHeight));
	vertices[2].position	= D3DXVECTOR2(0, mHeight);
	vertices[2].uv			= D3DXVECTOR2(0, 1);

	//vertices[3].position	= mViewport->TransformToViewport(D3DXVECTOR2(mWidth, mHeight));
	vertices[3].position	= D3DXVECTOR2(mWidth, mHeight);
	vertices[3].uv			= D3DXVECTOR2(1, 1);

	mBuffer = new VertexBuffer(mDevice);
	VertexBuffer::Data bufferDesc;

	bufferDesc.mUsage =					Usage::Default;
	bufferDesc.mTopology =				Topology::TriangleStrip;
	bufferDesc.mElementCount =			numVertices;
	bufferDesc.mElementSize =			sizeof(SpriteVertex);
	bufferDesc.mFirstElementPointer	=	vertices;

	mBuffer->SetData(bufferDesc, NULL);
}

void Sprite::CreateEffect()
{
	mEffect = new Effect(mDevice, "Resources/Effects/Sprite.fx");

	InputLayoutVector inputLayout;
	inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));
	inputLayout.push_back(InputLayoutElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT));

	mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
}

void Sprite::Draw(const D3DXVECTOR2& position)
{
	D3DXMATRIX model, vpInverse;
	D3DXMatrixIdentity(&model);
	//D3DXVECTOR2 transPos = mViewport->TransformToViewport(position);
	D3DXMatrixTranslation(&model, position.x, position.y, 0.0f);

	D3DXMatrixInverse(&vpInverse, NULL, &mViewport->GetMatrix());

	D3DXMatrixMultiply(&model, &model, &mViewport->GetMatrix());
	
	mEffect->SetVariable("gModel", model);
	mEffect->SetVariable("gTintColor", D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	mBuffer->Bind();

	for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
	{
		mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
		mBuffer->Draw();
	}
}
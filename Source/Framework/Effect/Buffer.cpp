#include "Buffer.hpp"

Buffer::Buffer(ID3D10Device* device) :
	mBuffer(NULL),
	mElementCount(0),
	mElementSize(0),
	mDevice(device)
{}

Buffer::~Buffer() throw()
{
	SafeRelease(mBuffer);
}

bool Buffer::SetBufferData(const Data& data, UINT bindFlags)
{
	SafeRelease(mBuffer);

	D3D10_BUFFER_DESC d3dBufferDescription;
	d3dBufferDescription.BindFlags = bindFlags;
	d3dBufferDescription.ByteWidth = data.mElementSize * data.mElementCount;
	d3dBufferDescription.MiscFlags = 0;
	SetAccessAndUsageFlags(d3dBufferDescription, data.mUsage);

	D3D10_SUBRESOURCE_DATA d3dData;
	d3dData.pSysMem = data.mFirstElementPointer;
	d3dData.SysMemPitch = 0;
	d3dData.SysMemSlicePitch = 0;

	HRESULT result = mDevice->CreateBuffer(&d3dBufferDescription, &d3dData, &mBuffer);
	if (FAILED(result))
		return false;

	mElementCount = data.mElementCount;
	mElementSize = data.mElementSize;

	return true;
}

void Buffer::SetAccessAndUsageFlags(D3D10_BUFFER_DESC& description, Usage::Usage usage)
{
	switch (usage)
	{
		case Usage::Immutable:
			description.Usage = D3D10_USAGE_IMMUTABLE;
			description.CPUAccessFlags = 0;
			break;
		case Usage::CPURead:
			description.Usage = D3D10_USAGE_DYNAMIC;
			description.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
			break;
		case Usage::CPUWrite:
			description.Usage = D3D10_USAGE_STAGING;
			description.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
			break;
		case Usage::Default:
		default:
			description.Usage = D3D10_USAGE_DEFAULT;
			description.CPUAccessFlags = 0;
	}
}



/**
	Vertex Buffer
*/

VertexBuffer::VertexBuffer(ID3D10Device* device) :
	Buffer(device) {}

bool VertexBuffer::SetBufferData(const Data& data, Topology::Topology topology)
{
	mTopology = topology;
	return Buffer::SetBufferData(data, D3D10_BIND_VERTEX_BUFFER);
}

void VertexBuffer::Bind()
{
	UINT offset = 0;
	mDevice->IASetVertexBuffers(0, 1, &mBuffer, &mElementSize, &offset);
	mDevice->IASetPrimitiveTopology(GetTopologyFlag(mTopology));
}

D3D10_PRIMITIVE_TOPOLOGY VertexBuffer::GetTopologyFlag(Topology::Topology topology)
{
	D3D10_PRIMITIVE_TOPOLOGY result;
	switch (topology)
	{
		case Topology::PointList:
			result = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		case Topology::LineStrip:
			result = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
			break;
		case Topology::LineList:
			result = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case Topology::TriangleStrip:
			result = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			break;
		case Topology::TriangleList:
			result = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		default:
			result = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
	}

	return result;
}



/**
	Index Buffer
*/

IndexBuffer::IndexBuffer(ID3D10Device* device) :
	Buffer(device) {}

bool IndexBuffer::SetBufferData(const Data& data)
{
	return Buffer::SetBufferData(data, D3D10_BIND_INDEX_BUFFER);
}

void IndexBuffer::Bind()
{
	UINT offset = 0;
	mDevice->IASetIndexBuffer(mBuffer, DXGI_FORMAT_R32_UINT, offset);
}
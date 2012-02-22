#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Global.hpp"
#include <d3d10.h>

/**
	Determine what access will be given to the buffer from the CPU/GPU.

	Default - GPU can read/write. CPU can not read or write.
	Immutable - GPU can read. CPU can only write at creation time. CPU can not read.
	CPURead - GPU can read, CPU can read.
	CPUWrite - GPU can read, CPU can write.
*/
namespace Usage
{
	enum Usage { Default, Immutable, CPURead, CPUWrite };
}

/**
	Determine how to interpret the vertices as primitives.

	PointList - Every vertex is rendered as a point.
	LineStrip - A line is drawn between each consequtive vertex.
	LineList - A line is drawn between every consequtive pair of vertices.
	TriangleStrip - A triangle is drawn between a vertex and the last two vertices.
	TriangleList - A triangle is drawn between every consequtive three vertices.
*/
namespace Topology
{
	enum Topology { PointList, LineStrip, LineList, TriangleStrip, TriangleList };
}




/**
	Contains data to be sent to the GPU
*/
class Buffer
{
public:
	/**
		A structure describing the buffer array.
	*/
	struct Data
	{
		Data() : mElementSize(0), mElementCount(0), mFirstElementPointer(NULL), mUsage(Usage::Default) {}

		unsigned int mElementSize;
		unsigned int mElementCount;
		void* mFirstElementPointer;
		Usage::Usage mUsage;
	};

	/**
		Virtual destructor
	*/
	virtual ~Buffer() throw();

	/**
		Bind the buffer to an input slot in the device.
		This is necessary to feed the buffer into the pipeline.
	*/
	virtual void Bind() = 0;
protected:
	Buffer(ID3D10Device* device);

	bool SetBufferData(const Data& data, UINT bindFlags);

	ID3D10Device* mDevice;
	ID3D10Buffer* mBuffer;
	unsigned int mElementSize;
	unsigned int mElementCount;
private:
	static void SetAccessAndUsageFlags(D3D10_BUFFER_DESC& description, Usage::Usage usage);
};

/**
	A buffer containing vertex data (vertices of geometry, along with their properties)
*/
class VertexBuffer : public Buffer
{
public:
	

	VertexBuffer(ID3D10Device* device);

	bool SetBufferData(const Data& data, Topology::Topology topology);
	void Bind();
private:
	Topology::Topology mTopology;

	static D3D10_PRIMITIVE_TOPOLOGY GetTopologyFlag(Topology::Topology topology);
};

/**
	A buffer containing indices into a vertex buffer, specifying which order to connect the primitives.
*/
class IndexBuffer : public Buffer
{
public:
	IndexBuffer(ID3D10Device* device);

	bool SetBufferData(const Data& data);
	void Bind();
};

#endif


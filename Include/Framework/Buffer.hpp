#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <D3DX10.h>
#include "Globals.hpp"

enum BufferType
{
	VertexBuffer,
	IndexBuffer
};

enum BufferUsage
{
	Buffer_Default,
	CPUWrite,
	CPURead
};

struct BufferInformation
{
	BufferType		type;
	UINT			elementSize;
	int				numberOfElements;
	void*			firstElementPointer;
	BufferUsage		usage;
};

class Buffer
{
public:
	Buffer();
	~Buffer();
	HRESULT Initialize(ID3D10Device* device, BufferInformation initDescription);
	void MakeActive();
	int GetNumberOfElements();
	int GetSize();
	//void Map();

private:
	ID3D10Device*			mDevice;
	ID3D10Buffer*			mBuffer;

	BufferInformation		mDescription;
};
#endif
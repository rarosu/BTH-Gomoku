#include "InputField.hpp"

const int InputField::C_NUM_VERTICES = 4;

InputField::InputField(ID3D10Device* device, InputManager* manager, InputReciever* reciever, 
	RECT position, GameFont* font)
{
	mDevice = device;
	mPosition = position;

	CreateBuffer();
	CreateEffect();

	manager->AddKeyListener(this);
	mReciever = reciever;
	mFont = font;
	
	mEffect->SetVectorVariable("consoleColor", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
}

void InputField::CreateBuffer()
{
	D3DXVECTOR2 vertices[C_NUM_VERTICES];

	vertices[0]	= TransformToViewport(D3DXVECTOR2((float)mPosition.left, (float)mPosition.top));
	vertices[1]	= TransformToViewport(D3DXVECTOR2((float)mPosition.right, (float)mPosition.top));
	vertices[2]	= TransformToViewport(D3DXVECTOR2((float)mPosition.left, (float)mPosition.bottom));
	vertices[3]	= TransformToViewport(D3DXVECTOR2((float)mPosition.right, (float)mPosition.bottom));

	mBuffer = new Buffer();
	BufferInformation bufferDesc;

	bufferDesc.type =					VertexBuffer;
	bufferDesc.usage =					Buffer_Default;
	bufferDesc.numberOfElements =		C_NUM_VERTICES;
	bufferDesc.firstElementPointer =	vertices;
	bufferDesc.elementSize =			sizeof(D3DXVECTOR2);
	bufferDesc.topology =				D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	mBuffer->Initialize(mDevice, bufferDesc);
}

void InputField::CreateEffect()
{
	// Create an array describing each of the elements of the vertex that are inputs to the vertex shader.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] = 
	{
		{ "POSITION",					// Semantic name, must be same as the vertex shader input semantic name
		  0,							// Semantic index, if one semantic name exists for more than one element
		  DXGI_FORMAT_R32G32_FLOAT,		// Format of the element, R32G32_FLOAT is a 32-bit 2D float vector
		  0,							// Input slot, of the 0-15 slots, through wich to send vertex data
		  0,							// AlignedByteOffset, bytes from start of the vertex to this component
		  D3D10_INPUT_PER_VERTEX_DATA,	// Input data class for this input slot
		  0 } 							// 0 when slot input data class is D3D10_INPUT_PER_VERTEX_DATA
	};

	mEffect = new Effect();
	mEffect->Initialize(mDevice, "Resources/Effects/Console.fx", vertexDesc,
		sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
}

void InputField::Draw()
{
	mBuffer->MakeActive();
	mEffect->MakeActive();

	for(UINT p = 0; p < mEffect->GetNumberOfPasses(); ++p)
	{
		mEffect->ApplyTechniquePass(p);
		mDevice->Draw(mBuffer->GetNumberOfElements(), 0);
	}

	int offset = 10;
	POINT position = { mPosition.left + offset, mPosition.top };
	mFont->WriteText(mStream.str(), position, D3DXCOLOR(0.0, 0.0, 0.0, 1.0));
}

void InputField::KeyPressed(int code)
{
	if(code == VK_RETURN)
	{
		if(mStream.str() != "")
		{
			if(mReciever)
			{
				mReciever->RecieveInput(mStream.str());
				mStream.str("");
			}
		}
	}
	else if(code == VK_BACK)
	{
		std::string temp = mStream.str();
		temp = temp.substr(0, temp.size() - 1);

		mStream.str("");
		mStream << temp;
	}
}

void InputField::KeyReleased(int code)
{
}

void InputField::CharEntered(unsigned char symbol)
{
	mStream << symbol;
}
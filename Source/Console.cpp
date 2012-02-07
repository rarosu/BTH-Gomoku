#include "Console.hpp"
#include <string>

const int Console::C_NUM_VERTICES = 4;

Console::Console(ID3D10Device* device, float screenWidth, float screenHeight, D3DXCOLOR bgColor, InputManager* manager)
	: mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0))
{
	mDevice = device;

	CreateBuffer(screenWidth, screenHeight);
	CreateEffect();

	mBounds.left = 0;
	mBounds.right = (LONG)screenWidth;
	mBounds.top = 0;
	mBounds.bottom = (LONG)(screenHeight / 2);

	int textHeight = 18;

	mFont = new GameFont(mDevice, "System", textHeight, false, true);
	mStream.str("");

	mMaxNumRows = (int)((screenHeight / 2 - textHeight) / textHeight);

	mInputField = new InputField(manager, this, D3DXVECTOR2(10.0f, mBounds.bottom - 20.0f), mFont);
	mEffect->SetVectorVariable("consoleColor", &(D3DXVECTOR4)bgColor);
}

void Console::CreateBuffer(float width, float height)
{
	D3DXVECTOR2 vertices[C_NUM_VERTICES];

	float screenX1 = -(width / 2);
	float screenX2 = width / 2;
	float screenY1 = height / 2;
	float screenY2 = 0;

	vertices[0]	= D3DXVECTOR2(screenX1, screenY1);
	vertices[1]	= D3DXVECTOR2(screenX2, screenY1);
	vertices[2]	= D3DXVECTOR2(screenX1, screenY2);
	vertices[3]	= D3DXVECTOR2(screenX2, screenY2);

	mVertexBuffer = new Buffer();
	BufferInformation bufferDesc;

	bufferDesc.type = VertexBuffer;
	bufferDesc.usage = Buffer_Default;
	bufferDesc.numberOfElements = C_NUM_VERTICES;
	bufferDesc.firstElementPointer = vertices;
	bufferDesc.elementSize = sizeof(D3DXVECTOR2);

	mVertexBuffer->Initialize(mDevice, bufferDesc);
}

void Console::CreateEffect()
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
	mEffect->Initialize(mDevice, "Effects/Console.fx", D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertexDesc,
		sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
}

// Update the console
void Console::Update(GameTime gameTime)
{
	if(!mIsToggled)
		return;
}

// Draw the console
void Console::Draw()
{
	if(!mIsToggled)
		return;

	mVertexBuffer->MakeActive();
	mEffect->MakeActive();

	for(UINT p = 0; p < mEffect->GetNumberOfPasses(); ++p)
	{
		mEffect->ApplyTechniquePass(p);
		mDevice->Draw(C_NUM_VERTICES, 0);
	}

	POINT position = { 10 , 0 };
	for(UINT i = std::max<int>(0, mOutput.size() - mMaxNumRows); i < mOutput.size(); ++i)
	{
		mFont->WriteText(mOutput[i], position, mTextColor);
		position.y += mFont->GetSize();
	}
	
	mInputField->Draw();
}

// Toggle showing of the console
void Console::Toggle()
{
	mIsToggled = !mIsToggled;
}

void Console::RecieveInput(std::string input)
{
	mOutput.push_back("> " + input);
}

//// When a key is pressed, act on it
//void Console::CharEntered(unsigned char key)
//{
//	if(!mIsToggled)
//		return;
//
//		mStream << key;
//}
//
//void Console::KeyPressed(int code)
//{
//	if(code == VK_RETURN)
//	{
//		if(mStream.str() != "")
//		{
//			mOutput.push_back("> " + mStream.str());
//			mStream.str("");
//		}
//	}
//	else if(code == VK_BACK)
//	{
//		std::string temp = mStream.str();
//		temp = temp.substr(0, temp.size() - 1);
//
//		mStream.str("");
//		mStream << temp;
//	}
//}
//
//void Console::KeyReleased(int code)
//{
//}


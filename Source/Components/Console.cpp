#include "Console.hpp"
#include <string>

namespace Components
{
	const int Console::C_NUM_VERTICES = 4;

	Console::Console(ID3D10Device* device, RECT position, D3DXCOLOR bgColor, InputManager* manager, UINT size)
		: mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0)), mIsToggled(true), mFirstShowRow(0), C_HISTORY_SIZE(size)
	{
		mDevice = device;
		mPositionRect = position;

		CreateBuffer();
		CreateEffect();

		int textHeight = 18;
		mFont = new GameFont(mDevice, "System", textHeight, false, true);
		mStream.str("");

		mMaxNumRows = (int)((GetHeight() - textHeight) / textHeight);

		RECT inputFieldPos = position;
		inputFieldPos.top = inputFieldPos.bottom - 20;

		mInputField = new InputField(mDevice, manager, this, inputFieldPos, mFont);
		mEffect->SetVectorVariable("consoleColor", &(D3DXVECTOR4)bgColor);
	}

	void Console::CreateBuffer()
	{
		D3DXVECTOR2 vertices[C_NUM_VERTICES];

		D3DXVECTOR2 point1 = D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.top);
		D3DXVECTOR2 point2 = D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.top);
		D3DXVECTOR2 point3 = D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.bottom);
		D3DXVECTOR2 point4 = D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.bottom);

		vertices[0]	= TransformToViewport(point1);
		vertices[1]	= TransformToViewport(point2);
		vertices[2]	= TransformToViewport(point3);
		vertices[3]	= TransformToViewport(point4);

		mVertexBuffer = new Buffer();
		BufferInformation bufferDesc;

		bufferDesc.type =					VertexBuffer;
		bufferDesc.usage =					Buffer_Default;
		bufferDesc.numberOfElements =		C_NUM_VERTICES;
		bufferDesc.firstElementPointer =	vertices;
		bufferDesc.elementSize =			sizeof(D3DXVECTOR2);
		bufferDesc.topology =				D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

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
		mEffect->Initialize(mDevice, "Resources/Effects/Console.fx", vertexDesc,
			sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
	}

	// Update the console
	void Console::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(!mIsToggled)
			return;

		if(currInputState.Keyboard.keyIsPressed[VK_UP] && !prevInputState.Keyboard.keyIsPressed[VK_UP])
		{
			--mFirstShowRow;
			mFirstShowRow = Clamp<int>(mFirstShowRow, 0, mOutput.size());
		}
		else if(currInputState.Keyboard.keyIsPressed[VK_DOWN] && !prevInputState.Keyboard.keyIsPressed[VK_DOWN])
		{
			++mFirstShowRow;
			mFirstShowRow = Clamp<int>(mFirstShowRow, 0, std::max<int>(0, mOutput.size() - mMaxNumRows));
		}
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
			mDevice->Draw(mVertexBuffer->GetNumberOfElements(), 0);
		}

		POINT position = { 3 , 0 };
		int start = std::max<int>(0, mFirstShowRow);
		int end = std::min<int>(mFirstShowRow + mMaxNumRows, mOutput.size());
		for(int i = start; i < end; ++i)
		{
			mFont->WriteText("> " + mOutput[i].text, position, mOutput[i].color);
			position.y += mFont->GetSize();
		}
	
		mInputField->Draw();
	}

	// Toggle showing of the console
	void Console::Toggle()
	{
		mIsToggled = !mIsToggled;
	}

	void Console::LostFocus()
	{
	}

	void Console::GotFocus()
	{
	}

	void Console::RecieveInput(std::string input)
	{
		TextLine text;
		text.text = input;
		text.color = mTextColor;

		if(mOutput.size() >= C_HISTORY_SIZE)
			mOutput.erase(mOutput.begin());

		mOutput.push_back(text);

		if(mOutput.size() > (UINT)mMaxNumRows)
			mFirstShowRow = mOutput.size() - mMaxNumRows;
	}

	void Console::SetTextColor(D3DXCOLOR newColor)
	{
		mTextColor = newColor;
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
}
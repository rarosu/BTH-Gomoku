#include "InputField.hpp"

namespace Components
{
	const int InputField::C_NUM_VERTICES = 4;
	const float InputField::C_MARKER_SPEED = 500;

	InputField::InputField(ID3D10Device* device, InputSubscription* manager, InputReciever* reciever, 
		RECT position, GameFont* font)
		: mDevice(device), mManager(manager), mBuffer(NULL), mEffect(NULL), mFont(NULL), 
		  mReciever(NULL), mShowMarker(true), mMSSinceBlink(0.0f)
	{
		mPositionRect = position;

		CreateBuffer();
		CreateEffect();

		mManager->AddKeyListener(this);
		mReciever = reciever;
		mFont = font;
	
		mEffect->SetVectorVariable("bgColor", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	
	InputField::~InputField()
	{
		SafeDelete(mBuffer);
		SafeDelete(mEffect);
	}

InputField::~InputField() throw()
{
	mInputManager->RemoveKeyListener(this);

	SafeDelete(mBuffer);
	SafeDelete(mEffect);
}

void InputField::CreateBuffer()
{
	D3DXVECTOR2 vertices[C_NUM_VERTICES];
		vertices[0]	= TransformToViewport(D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.top));
		vertices[1]	= TransformToViewport(D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.top));
		vertices[2]	= TransformToViewport(D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.bottom));
		vertices[3]	= TransformToViewport(D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.bottom));

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
		mEffect->Initialize(mDevice, "Resources/Effects/Basic2D.fx", vertexDesc,
			sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
	}

	void InputField::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		mMSSinceBlink += (float)gameTime.GetTimeSinceLastTick().Milliseconds;

		if(mMSSinceBlink >= C_MARKER_SPEED)
		{
			mShowMarker = !mShowMarker;
			mMSSinceBlink -= C_MARKER_SPEED;
		}
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
		POINT position = { mPositionRect.left + offset, mPositionRect.top };

		std::string marker = " ";
		if(mShowMarker)
			marker = "|";
		//mFont->WriteText(mStream.str() + marker, position, D3DXCOLOR(0.0, 0.0, 0.0, 1.0));
		mFont->WriteText(mFirstString.str() + marker + mLastString.str(), 
						 position, D3DXCOLOR(0.0, 0.0, 0.0, 1.0));
	}

	void InputField::LostFocus()
	{
	}

	void InputField::GotFocus()
	{
	}

void InputField::KeyPressed(int code)
	{
		std::string first, letter, last;

		switch(code)
		{
			case VK_RETURN:
				if(mFirstString.str() != "" || mLastString.str() != "")
				{
					if(mReciever)
					{
						//mReciever->RecieveInput(mStream.str());
						//mStream.str("");
						mReciever->RecieveInput(mFirstString.str() + mLastString.str());
						mFirstString.str("");
						mLastString.str("");
					}
				}
				break;
			case VK_BACK:
				first = mFirstString.str();
				first = first.substr(0, first.size() - 1);

				mFirstString.str("");
				mFirstString << first;
				break;
			case VK_LEFT:
				if(mFirstString.str() != "")
				{
					first = mFirstString.str();
					last = mLastString.str();
					letter = first.substr(first.size() - 1, first.size());
					first = first.substr(0, first.size() - 1);

					mFirstString.str("");
					mFirstString << first;
					mLastString.str("");
					mLastString << letter << last;
				}
				break;
			case VK_RIGHT:
				if(mLastString.str() != "")
				{
					first = mFirstString.str();
					last = mLastString.str();
					letter = last.substr(0, 1);
					last = last.substr(1, last.size());

					mFirstString.str("");
					mFirstString << first << letter;
					mLastString.str("");
					mLastString << last;
				}
				break;
		}
	}

	void InputField::KeyReleased(int code)
	{
	}

	void InputField::CharEntered(unsigned char symbol)
	{
		//mStream << symbol;
		mFirstString << symbol;
	}
}
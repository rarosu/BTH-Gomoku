#include "StandardButton.hpp"

namespace Components
{
	StandardButton::StandardButton(InputSubscription* manager)
		: Clickable(manager),
		mDevice(NULL), mBuffer(NULL), mEffect(NULL), mFont (NULL), mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0))
	{
	}

	void StandardButton::Initialize(ID3D10Device* device, RECT position, std::string caption)
	{
		mDevice = device;
		mCaption = caption;
		mIdleColor = D3DXCOLOR(0.5f, 0.5f, 0.2f, 1.0f);
		mHoverColor = mIdleColor * 0.8f;
		mHoverColor.a = mIdleColor.a;
		mActiveColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, mIdleColor.a);

		mPositionRect = position;

		CreateBuffer();
		CreateEffect();

		mFont = new GameFont(mDevice, "Comic Sans", 18);

		ID3D10ShaderResourceView* resource = NULL;
		D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/button.png", NULL, NULL, &resource, NULL );
		mEffect->SetResourceVariable("textureBase", resource);
		mEffect->SetVectorVariable("buttonColor", &(D3DXVECTOR4)mIdleColor);
	}

	void StandardButton::CreateBuffer()
	{
		const int numVertices = 4;
		ButtonVertex vertices[numVertices];

		vertices[0].position	= TransformToViewport(D3DXVECTOR2((float)mPositionRect.left, 
																  (float)mPositionRect.top));
		vertices[0].uv			= D3DXVECTOR2(0, 0);

		vertices[1].position	= TransformToViewport(D3DXVECTOR2((float)mPositionRect.right, 
																  (float)mPositionRect.top));
		vertices[1].uv			= D3DXVECTOR2(1, 0);

		vertices[2].position	= TransformToViewport(D3DXVECTOR2((float)mPositionRect.left, 
																  (float)mPositionRect.bottom));
		vertices[2].uv			= D3DXVECTOR2(0, 1);

		vertices[3].position	= TransformToViewport(D3DXVECTOR2((float)mPositionRect.right, 
																  (float)mPositionRect.bottom));
		vertices[3].uv			= D3DXVECTOR2(1, 1);

		mBuffer = new Buffer();
		BufferInformation bufferDesc;

		bufferDesc.type =					VertexBuffer;
		bufferDesc.usage =					Buffer_Default;
		bufferDesc.numberOfElements =		numVertices;
		bufferDesc.firstElementPointer =	vertices;
		bufferDesc.elementSize =			sizeof(ButtonVertex);
		bufferDesc.topology =				D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		mBuffer->Initialize(mDevice, bufferDesc);
	}

	void StandardButton::CreateEffect()
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
			  0 }, 							// 0 when slot input data class is D3D10_INPUT_PER_VERTEX_DATA
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(D3DXVECTOR2), D3D10_INPUT_PER_VERTEX_DATA, 0 }
		};

		mEffect = new Effect();
		mEffect->Initialize(mDevice, "Resources/Effects/Button.fx", vertexDesc,
			sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
	}

	void StandardButton::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		Clickable::Update(gameTime, currInputState, prevInputState);
	}

	void StandardButton::Draw()
	{
		mBuffer->MakeActive();
		mEffect->MakeActive();

		for(UINT p = 0; p < mEffect->GetNumberOfPasses(); ++p)
		{
			mEffect->ApplyTechniquePass(p);
			mDevice->Draw(mBuffer->GetNumberOfElements(), 0);
		}
		mFont->WriteText(mCaption, &mPositionRect, mTextColor, GameFont::Center, GameFont::Middle);
	}

	void StandardButton::MouseEntered()
	{
		if(IsPressed())
			mEffect->SetVectorVariable("buttonColor", &(D3DXVECTOR4)mActiveColor);
		else
			mEffect->SetVectorVariable("buttonColor", &(D3DXVECTOR4)mHoverColor);
	}

	void StandardButton::MouseExited()
	{
		mEffect->SetVectorVariable("buttonColor", &(D3DXVECTOR4)mIdleColor);
	}

	void StandardButton::MousePressed(int buttonIndex)
	{
		mEffect->SetVectorVariable("buttonColor", &(D3DXVECTOR4)mActiveColor);
	}

	void StandardButton::MouseReleased(int buttonIndex)
	{
		mEffect->SetVectorVariable("buttonColor", &(D3DXVECTOR4)mHoverColor);
		MessageBox(NULL, "Clicked!", "Test", 0);
	}
}
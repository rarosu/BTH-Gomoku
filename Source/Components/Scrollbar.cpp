#include "Scrollbar.hpp"

namespace Components
{
	Scrollbar::Scrollbar(InputSubscription* manager, Scrollable* scrollable)
		: Clickable(manager), mScrollable(scrollable)
	{
		mBtnUp = new Button(manager);
		mBtnDown = new Button(manager);
		manager->AddMouseListener(this);
	}

	Scrollbar::~Scrollbar() throw()
	{
		SafeDelete(mBuffer);
		SafeDelete(mEffect);
		SafeDelete(mBtnUp);
		SafeDelete(mBtnDown);
	}

	void Scrollbar::Initialize(ID3D10Device* device, RECT position)
	{
		mDevice = device;
		mPositionRect = position;
		LONG buttonHeight = position.right - position.left;	// Button height is same as its width

		CreateBuffer((float)buttonHeight / 2);
		CreateEffect();

		Button::Graphics graphics1, graphics2;
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/btnUIdle.png", NULL, NULL, 
											   &graphics1.textureUp, NULL);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/btnUPressed.png", NULL, NULL, 
											   &graphics1.textureDown, NULL);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/btnDIdle.png", NULL, NULL, 
											   &graphics2.textureUp, NULL);
		D3DX10CreateShaderResourceViewFromFile(device, "Resources/Textures/btnDPressed.png", NULL, NULL, 
											   &graphics2.textureDown, NULL);

		RECT pos1 = { position.left, position.top, position.right, position.top + buttonHeight };

		RECT pos2 = { position.left, position.bottom - buttonHeight, position.right, position.bottom };
		
		mBtnUp->Initialize(device, pos1, graphics1);
		mBtnDown->Initialize(device, pos2, graphics2);

		mEffect->SetVectorVariable("bgColor", &(D3DXVECTOR4)C_COLOR_COMPONENT_BG);
	}

	void Scrollbar::CreateBuffer(float offset)
	{
		const int numVertices = 4;
		D3DXVECTOR2 vertices[numVertices];

		D3DXVECTOR2 point1 = D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.top + offset);
		D3DXVECTOR2 point2 = D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.top + offset);
		D3DXVECTOR2 point3 = D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.bottom - offset);
		D3DXVECTOR2 point4 = D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.bottom - offset);

		vertices[0]	= TransformToViewport(point1);
		vertices[1]	= TransformToViewport(point2);
		vertices[2]	= TransformToViewport(point3);
		vertices[3]	= TransformToViewport(point4);

		mBuffer = new Buffer();
		BufferInformation bufferDesc;

		bufferDesc.type						= VertexBuffer;
		bufferDesc.usage					= Buffer_Default;
		bufferDesc.numberOfElements			= numVertices;
		bufferDesc.firstElementPointer		= vertices;
		bufferDesc.elementSize				= sizeof(D3DXVECTOR2);
		bufferDesc.topology					= D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		mBuffer->Initialize(mDevice, bufferDesc);
	}
	
	void Scrollbar::CreateEffect()
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

	void Scrollbar::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		Clickable::Update(gameTime, currInputState, prevInputState);

		mBtnUp->Update(gameTime, currInputState, prevInputState);
		mBtnDown->Update(gameTime, currInputState, prevInputState);
	}

	void Scrollbar::Draw()
	{
		mBuffer->MakeActive();
		mEffect->MakeActive();

		for(UINT p = 0; p < mEffect->GetNumberOfPasses(); ++p)
		{
			mEffect->ApplyTechniquePass(p);
			mDevice->Draw(mBuffer->GetNumberOfElements(), 0);
		}

		mBtnUp->Draw();
		mBtnDown->Draw();
	}

	void Scrollbar::LostFocus()
	{
	}
	
	void Scrollbar::GotFocus()
	{
	}

	void Scrollbar::MouseEntered()
	{
	}

	void Scrollbar::MouseExited()
	{
	}

	void Scrollbar::MousePressed(int buttonIndex)
	{
	}

	void Scrollbar::MouseReleased(int buttonIndex)
	{
		if(mBtnUp->IsHovered() && mScrollable)
			mScrollable->Scroll(true);
		else if(mBtnDown->IsHovered() && mScrollable)
			mScrollable->Scroll(false);
	}
}
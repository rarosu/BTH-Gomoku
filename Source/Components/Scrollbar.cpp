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

		mEffect->SetVariable("bgColor", &(D3DXVECTOR4)C_COLOR_COMPONENT_BG);
	}

	void Scrollbar::CreateBuffer(float offset)
	{
		const int numVertices = 4;
		D3DXVECTOR2 vertices[numVertices];

		D3DXVECTOR2 point1 = D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.top + offset);
		D3DXVECTOR2 point2 = D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.top + offset);
		D3DXVECTOR2 point3 = D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.bottom - offset);
		D3DXVECTOR2 point4 = D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.bottom - offset);

		vertices[0]	= sViewport->TransformToViewport(point1);
		vertices[1]	= sViewport->TransformToViewport(point2);
		vertices[2]	= sViewport->TransformToViewport(point3);
		vertices[3]	= sViewport->TransformToViewport(point4);

		mBuffer = new VertexBuffer(mDevice);
		VertexBuffer::Data bufferDesc;

		bufferDesc.mUsage					= Usage::Default;
		bufferDesc.mTopology				= Topology::TriangleStrip;
		bufferDesc.mElementSize				= sizeof(D3DXVECTOR2);
		bufferDesc.mElementCount			= numVertices;
		bufferDesc.mFirstElementPointer		= vertices;

		mBuffer->SetData(bufferDesc, NULL);
	}
	
	void Scrollbar::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Basic2D.fx");
		
		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
	}

	void Scrollbar::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		Clickable::Update(gameTime, currInputState, prevInputState);

		mBtnUp->Update(gameTime, currInputState, prevInputState);
		mBtnDown->Update(gameTime, currInputState, prevInputState);
	}

	void Scrollbar::Draw()
	{
		mBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
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
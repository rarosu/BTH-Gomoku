#include "Button.hpp"

namespace Components
{
	Button::Graphics::Graphics()
	{
		idleColor = activeColor = hoverColor = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
		textureUp = NULL;
		textureDown = NULL;
		textureHover = NULL;
	}

	Button::Button(ComponentGroup* ownerGroup)
		: Clickable(ownerGroup),
		mDevice(NULL), mBuffer(NULL), mEffect(NULL)
	{
	}

	void Button::Initialize(ID3D10Device* device, RECT position, Graphics buttonGraphics)
	{
		mDevice = device;
		mGraphics = buttonGraphics;
		mPositionRect = position;

		CreateBuffer();
		CreateEffect();

		if(mGraphics.textureUp == NULL)
			D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/button.png", NULL, NULL, &mGraphics.textureUp, NULL);

		mEffect->SetVariable("textureBase", mGraphics.textureUp);
		mEffect->SetVariable("buttonColor", (D3DXVECTOR4)mGraphics.idleColor);
	}

	void Button::CreateBuffer()
	{
		const int numVertices = 4;
		ButtonVertex vertices[numVertices];

		vertices[0].position	= sViewport->TransformToViewport(D3DXVECTOR2((float)mPositionRect.left, 
																			 (float)mPositionRect.top));
		vertices[0].uv			= D3DXVECTOR2(0, 0);

		vertices[1].position	= sViewport->TransformToViewport(D3DXVECTOR2((float)mPositionRect.right, 
																			 (float)mPositionRect.top));
		vertices[1].uv			= D3DXVECTOR2(1, 0);

		vertices[2].position	= sViewport->TransformToViewport(D3DXVECTOR2((float)mPositionRect.left, 
																			 (float)mPositionRect.bottom));
		vertices[2].uv			= D3DXVECTOR2(0, 1);

		vertices[3].position	= sViewport->TransformToViewport(D3DXVECTOR2((float)mPositionRect.right, 
																			 (float)mPositionRect.bottom));
		vertices[3].uv			= D3DXVECTOR2(1, 1);

		mBuffer = new VertexBuffer(mDevice);
		VertexBuffer::Data bufferDesc;

		bufferDesc.mUsage =					Usage::Default;
		bufferDesc.mTopology =				Topology::TriangleStrip;
		bufferDesc.mElementCount =			numVertices;
		bufferDesc.mElementSize =			sizeof(ButtonVertex);
		bufferDesc.mFirstElementPointer	=	vertices;

		mBuffer->SetData(bufferDesc, NULL);
	}

	void Button::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Button.fx");

		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));
		inputLayout.push_back(InputLayoutElement("UV", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
	}

	void Button::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		Clickable::Update(gameTime, currInputState, prevInputState);
	}

	void Button::Draw()
	{
		mBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
		}
	}
	
	void Button::LostFocus()
	{
	}

	void Button::GotFocus()
	{
	}

	void Button::MouseEntered()
	{
		if(IsPressed())
		{
			mEffect->SetVariable("buttonColor", (D3DXVECTOR4)mGraphics.activeColor);
			if(mGraphics.textureDown)
				mEffect->SetVariable("textureBase", mGraphics.textureDown);
		}
		else
		{
			mEffect->SetVariable("buttonColor", (D3DXVECTOR4)mGraphics.hoverColor);
			if(mGraphics.textureHover)
				mEffect->SetVariable("textureBase", mGraphics.textureHover);

		}
	}

	void Button::MouseExited()
	{
		mEffect->SetVariable("buttonColor", (D3DXVECTOR4)mGraphics.idleColor);
		mEffect->SetVariable("textureBase", mGraphics.textureUp);
	}

	void Button::MousePressed(int buttonIndex)
	{
		mEffect->SetVariable("buttonColor", (D3DXVECTOR4)mGraphics.activeColor);
		if(mGraphics.textureDown)
			mEffect->SetVariable("textureBase", mGraphics.textureDown);
	}

	void Button::MouseReleased(int buttonIndex)
	{
		mEffect->SetVariable("buttonColor", (D3DXVECTOR4)mGraphics.hoverColor);
		if(mGraphics.textureHover)
				mEffect->SetVariable("textureBase", mGraphics.textureHover);
		else
			mEffect->SetVariable("textureBase", mGraphics.textureUp);
	}
}
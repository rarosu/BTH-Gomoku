#include "Console.hpp"
#include <string>


namespace Components
{
	const int Console::C_NUM_VERTICES = 4;

	Console::Console(ID3D10Device* device, ComponentGroup* ownerGroup, RECT position, D3DXCOLOR bgColor, UINT size)
		: ComponentGroup(ownerGroup, "Console"),
		  mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0)), mFirstShowRow(0), 
		  C_HISTORY_SIZE(size)
	{
		mDevice = device;
		mPositionRect = position;

		CreateBuffer();
		CreateEffect();

		int textHeight = 18;
		mFont = new GameFont(mDevice, "System", textHeight, false, true);
		mStream.str("");

		mMaxNumRows = (int)((GetHeight() - textHeight) / textHeight);

		RECT inputFieldPos = mPositionRect;
		inputFieldPos.top = inputFieldPos.bottom - 20;

		int scrollWidth = 20;
		RECT scrollbarPos = { mPositionRect.right - scrollWidth, mPositionRect.top, 
							  mPositionRect.right, mPositionRect.bottom };
		mScrollbar = new Scrollbar(this, this);
		mScrollbar->Initialize(mDevice, scrollbarPos);
		mInputField = new InputField(mDevice, this, this, inputFieldPos, mFont);
		mEffect->SetVariable("bgColor", (D3DXVECTOR4)C_COLOR_WINDOW_BG);

		SetFocus(mInputField);
}


	Console::~Console() throw()
	{
		SafeDelete(mEffect);
		SafeDelete(mVertexBuffer);
	}

	void Console::CreateBuffer()
	{
		D3DXVECTOR2 vertices[C_NUM_VERTICES];

		D3DXVECTOR2 point1 = D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.top);
		D3DXVECTOR2 point2 = D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.top);
		D3DXVECTOR2 point3 = D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.bottom);
		D3DXVECTOR2 point4 = D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.bottom);

		vertices[0]	= sViewport->TransformToViewport(point1);
		vertices[1]	= sViewport->TransformToViewport(point2);
		vertices[2]	= sViewport->TransformToViewport(point3);
		vertices[3]	= sViewport->TransformToViewport(point4);

		mVertexBuffer = new VertexBuffer(mDevice);
		VertexBuffer::Data bufferDesc;

		bufferDesc.mUsage					= Usage::Default;
		bufferDesc.mTopology				= Topology::TriangleStrip;
		bufferDesc.mElementCount			= C_NUM_VERTICES;
		bufferDesc.mElementSize				= sizeof(D3DXVECTOR2);
		bufferDesc.mFirstElementPointer		= vertices;

		mVertexBuffer->SetData(bufferDesc, NULL);
	}

	void Console::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Basic2D.fx");

		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
	}

	// Update the console
	void Console::Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState)
	{
		if(!IsVisible())
			return;

		if(currInputState.Keyboard.keyIsPressed[VK_UP] && !prevInputState.Keyboard.keyIsPressed[VK_UP])
		{
			Scroll(true);
		}
		else if(currInputState.Keyboard.keyIsPressed[VK_DOWN] && !prevInputState.Keyboard.keyIsPressed[VK_DOWN])
		{
			Scroll(false);
		}

		ComponentGroup::Update(gameTime, currInputState, prevInputState);
	}

	// Draw the console
	void Console::Draw()
	{
		if(!IsVisible())
			return;

		mVertexBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mVertexBuffer->Draw();
		}

		POINT position = { 3 , 0 };
		int start = std::max<int>(0, mFirstShowRow);
		int end = std::min<int>(mFirstShowRow + mMaxNumRows, mOutput.size());
		for(int i = start; i < end; ++i)
		{
			mFont->WriteText("> " + mOutput[i].text, position, mOutput[i].color);
			position.y += mFont->GetSize();
		}

		ComponentGroup::Draw();
	}

	// Toggle showing of the console
	void Console::Toggle()
	{
		if(IsVisible())
		{
			SetVisible(false);
			LoseFocus();
		}
		else
		{
			SetVisible(true);
			SetFocusThis();
		}
	}

	void Console::LostFocus()
	{
	}

	void Console::GotFocus()
	{
	}

	void Console::Scroll(bool isUp)
	{
		if(isUp)
		{
			--mFirstShowRow;
			mFirstShowRow = Clamp<int>(mFirstShowRow, 0, mOutput.size());
		}
		else
		{
			++mFirstShowRow;
			mFirstShowRow = Clamp<int>(mFirstShowRow, 0, std::max<int>(0, mOutput.size() - mMaxNumRows));
		}
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

	// DEBUG
	std::string Console::GetName()
	{
		return "Console";
	}
}
#include "Console.hpp"
#include <string>


namespace Components
{
	const int Console::C_NUM_VERTICES = 4;

	Console::Console(ID3D10Device* device, RECT position, D3DXCOLOR bgColor, InputSubscription* manager, UINT size)
		: mTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0)), mInputManager(manager), mIsToggled(false), mFirstShowRow(0), 
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
		mScrollbar = new Scrollbar(mInputManager, this);
		mScrollbar->Initialize(mDevice, scrollbarPos);
		mInputField = new InputField(mDevice, mInputManager, this, inputFieldPos, mFont);
		mEffect->SetVariable("bgColor", &(D3DXVECTOR4)C_COLOR_WINDOW_BG);
}


	Console::~Console() throw()
	{
		SafeDelete(mEffect);
		SafeDelete(mVertexBuffer);
		SafeDelete(mInputField);
		SafeDelete(mScrollbar);
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
		if(!mIsToggled)
			return;

		if(currInputState.Keyboard.keyIsPressed[VK_UP] && !prevInputState.Keyboard.keyIsPressed[VK_UP])
		{
			Scroll(true);
		}
		else if(currInputState.Keyboard.keyIsPressed[VK_DOWN] && !prevInputState.Keyboard.keyIsPressed[VK_DOWN])
		{
			Scroll(false);
		}

		mInputField->Update(gameTime, currInputState, prevInputState);
		mScrollbar->Update(gameTime, currInputState, prevInputState);
	}

	// Draw the console
	void Console::Draw()
	{
		if(!mIsToggled)
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
	
		mInputField->Draw();
		mScrollbar->Draw();
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

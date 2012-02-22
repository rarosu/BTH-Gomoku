#include "InputField.hpp"

namespace Components
{
	const int InputField::C_NUM_VERTICES = 4;
	const float InputField::C_MARKER_SPEED = 500;

	InputField::InputField(ID3D10Device* device, InputSubscription* manager, InputReceiver* receiver, 
		RECT position, GameFont* font)
		: mDevice(device), mManager(manager), mBuffer(NULL), mEffect(NULL), mFont(font), 
		  mReceiver(receiver), mShowMarker(true), mMSSinceBlink(0.0f)
	{
		mPositionRect = position;

		CreateBuffer();
		CreateEffect();

		mManager->AddKeyListener(this);
	
		mEffect->SetVariable("bgColor", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	InputField::~InputField() throw()
	{
		mManager->RemoveKeyListener(this);

		SafeDelete(mBuffer);
		SafeDelete(mEffect);
	}

	void InputField::CreateBuffer()
	{
		D3DXVECTOR2 vertices[C_NUM_VERTICES];
		vertices[0]	= sViewport->TransformToViewport(D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.top));
		vertices[1]	= sViewport->TransformToViewport(D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.top));
		vertices[2]	= sViewport->TransformToViewport(D3DXVECTOR2((float)mPositionRect.left, (float)mPositionRect.bottom));
		vertices[3]	= sViewport->TransformToViewport(D3DXVECTOR2((float)mPositionRect.right, (float)mPositionRect.bottom));

		mBuffer = new VertexBuffer(mDevice);
		VertexBuffer::Data bufferDesc;

		bufferDesc.mUsage =					Usage::Default;
		bufferDesc.mTopology =				Topology::TriangleStrip;
		bufferDesc.mElementCount =			C_NUM_VERTICES;
		bufferDesc.mElementSize =			sizeof(D3DXVECTOR2);
		bufferDesc.mFirstElementPointer =	vertices;

		mBuffer->SetData(bufferDesc, NULL);
	}

	void InputField::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Basic2D.fx");
		
		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
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
		mBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
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

	void InputField::KeyPressed(int code, const InputState& currentState)
	{
		std::string first, letter, last;

		switch(code)
		{
			case VK_RETURN:
				if(mFirstString.str() != "" || mLastString.str() != "")
				{
					if(mReceiver)
					{
						//mReceiver->RecieveInput(mStream.str());
						//mStream.str("");
						mReceiver->RecieveInput(mFirstString.str() + mLastString.str());
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

	void InputField::KeyReleased(int code, const InputState& currentState)
	{
	}

	void InputField::CharEntered(unsigned char symbol, const InputState& currentState)
	{
		//mStream << symbol;
		mFirstString << symbol;
	}
}
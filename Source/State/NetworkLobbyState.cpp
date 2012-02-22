#include "NetworkLobbyState.hpp"

namespace State
{
	NetworkLobbyState::NetworkLobbyState(StateID id, ID3D10Device* device, InputSubscription* manager, int width, int height) 
		: ApplicationState(id), mDevice(device), mEffect(NULL), mBuffer(NULL)
	{
		CreateBuffer((float)width, (float)height);
		CreateEffect();

		ID3D10ShaderResourceView* texture;
		D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/titleScreenBig.png", NULL, NULL, 
											   &texture, NULL);
		mEffect->SetVariable("textureBG", texture);

		const std::string btnCaptions[] = { "Start Game", "Back To Menu" };
		LONG centerX = (LONG)width / 4 + 100;
		LONG centerY = (LONG)height / 2;
		const int padding = 20;

		for(int i = 0; i < NLobbyButton::Count; ++i)
		{
			mButtons.push_back(new Components::TextButton(manager));
		
			RECT buttonPos = { centerX - 96, centerY - 24, centerX + 96, centerY + 24 };
			mButtons.at(i)->Initialize(mDevice, buttonPos, btnCaptions[i]);
			centerY += 48 + padding;
		}
	}

	NetworkLobbyState::~NetworkLobbyState() throw()
	{
		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			SafeDelete(mButtons.at(i));
		}
	}

	void NetworkLobbyState::CreateBuffer(float width, float height)
	{
		const int numVertices = 4;
		bgVertex vertices[numVertices];

		vertices[0].position = sViewport->TransformToViewport(D3DXVECTOR2(0, 0));
		vertices[0].uv = D3DXVECTOR2(0, 0);
		vertices[1].position = sViewport->TransformToViewport(D3DXVECTOR2(width, 0));
		vertices[1].uv = D3DXVECTOR2(1, 0);
		vertices[2].position = sViewport->TransformToViewport(D3DXVECTOR2(0, height));
		vertices[2].uv = D3DXVECTOR2(0, 1);
		vertices[3].position = sViewport->TransformToViewport(D3DXVECTOR2(width, height));
		vertices[3].uv = D3DXVECTOR2(1, 1);

		mBuffer = new VertexBuffer(mDevice);
		VertexBuffer::Data bufferDesc;

		bufferDesc.mUsage					= Usage::Default;
		bufferDesc.mTopology				= Topology::TriangleStrip;
		bufferDesc.mElementCount			= numVertices;
		bufferDesc.mFirstElementPointer		= vertices;
		bufferDesc.mElementSize				= sizeof(bgVertex);

		mBuffer->SetData(bufferDesc, NULL);
	}
	
	void NetworkLobbyState::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Background.fx");
		
		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));
		inputLayout.push_back(InputLayoutElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
	}

	void NetworkLobbyState::OnStatePushed()
	{
		for(int i = 0; i < NLobbyButton::Count; ++i)
		{
			sInputManager->AddMouseListener(mButtons[i]);
		}
	}

	void NetworkLobbyState::OnStatePopped()
	{
		for(int i = 0; i < NLobbyButton::Count; ++i)
		{
			sInputManager->RemoveMouseListener(mButtons[i]);
		}
	}

	void NetworkLobbyState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{

		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE])
			QuitApplication();

		if(mButtons.at(NLobbyButton::Game)->GetAndResetClickStatus())
			ChangeState(C_STATE_IN_GAME);
		if(mButtons.at(NLobbyButton::Back)->GetAndResetClickStatus())
			ChangeState(C_STATE_MENU);

		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			mButtons.at(i)->Update(gameTime, currInput, prevInput);
		}
	}

	void NetworkLobbyState::Draw()
	{
		mBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
		}

		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			mButtons.at(i)->Draw();
		}
	}
}
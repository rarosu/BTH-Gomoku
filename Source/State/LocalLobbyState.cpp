#include "LocalLobbyState.hpp"

namespace State
{
	LocalLobbyState::LocalLobbyState(StateID id, ID3D10Device* device, int width, int height) 
		: ApplicationState(id), mDevice(device), mEffect(NULL), mBuffer(NULL), mTitle(NULL), mComponents(NULL)
	{
		CreateBuffer((float)width, (float)height);
		CreateEffect();

		mComponents = new Components::ComponentGroup(sRootComponentGroup);
		sInputManager->AddKeyListener(mComponents);
		sInputManager->AddMouseListener(mComponents);

		ID3D10ShaderResourceView* texture;
		D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/marbleBG1422x800.png", NULL, NULL, 
											   &texture, NULL);
		mEffect->SetVariable("textureBG", texture);

		const std::string btnCaptions[] = { "Start Game", "Back To Menu" };
		LONG centerX = (LONG)width / 2;
		LONG centerY = (LONG)height / 2;
		const int padding = 20;

		for(int i = 0; i < LLobbyButton::Count; ++i)
		{
			mButtons.push_back(new Components::TextButton(mComponents));
		
			RECT buttonPos = { centerX - 96, centerY - 24, centerX + 96, centerY + 24 };
			mButtons.at(i)->Initialize(mDevice, buttonPos, btnCaptions[i]);
			centerY += 48 + padding;
		}

		int lableX = sViewport->GetWidth() / 2;
		RECT labelPos = { lableX - 360, 100, lableX + 360, 200 };
		mTitle = new Components::Label(mDevice, mComponents, "CREATE A LOCAL GAME", labelPos);
	}

	LocalLobbyState::~LocalLobbyState() throw()
	{
		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			SafeDelete(mButtons.at(i));
		}
	}

	void LocalLobbyState::CreateBuffer(float width, float height)
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
		bufferDesc.mElementSize				= sizeof(bgVertex);
		bufferDesc.mFirstElementPointer		= vertices;

		mBuffer->SetData(bufferDesc, NULL);
	}
	
	void LocalLobbyState::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Background.fx");
		
		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));
		inputLayout.push_back(InputLayoutElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
	}

	void LocalLobbyState::OnStatePushed()
	{
		for(int i = 0; i < LLobbyButton::Count; ++i)
		{
			sInputManager->AddMouseListener(mButtons[i]);
		}
	}

	void LocalLobbyState::OnStatePopped()
	{
		for(int i = 0; i < LLobbyButton::Count; ++i)
		{
			sInputManager->RemoveMouseListener(mButtons[i]);
		}
	}

	void LocalLobbyState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{

		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE])
			QuitApplication();

		if(mButtons.at(LLobbyButton::Game)->GetAndResetClickStatus())
			ChangeState(C_STATE_IN_GAME);
		if(mButtons.at(LLobbyButton::Back)->GetAndResetClickStatus())
			ChangeState(C_STATE_MENU);

		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			mButtons.at(i)->Update(gameTime, currInput, prevInput);
		}
	}

	void LocalLobbyState::Draw()
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

		mTitle->Draw();
	}
}
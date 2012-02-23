#include "MenuState.hpp"

namespace State
{
	MenuState::MenuState(StateID id, ID3D10Device* device, int width, int height) 
		: ApplicationState(id), mDevice(device),  mEffect(NULL), mBuffer(NULL), mDragonAgeMenu(NULL)
	{
		CreateBuffer((float)width, (float)height);
		CreateEffect();

		ID3D10ShaderResourceView* texture;
		D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/titleScreen1422x800.png", NULL, NULL, 
											   &texture, NULL);
		mEffect->SetVariable("textureBG", texture);

		const std::string btnCaptions[] = { "Local Game", "Network Game", "Options", "Exit" };
		LONG centerX = (LONG)width / 4;
		LONG centerY = (LONG)height / 2;
		const int padding = 20;

		for(int i = 0; i < MenuButton::Count; ++i)
		{
			mButtons.push_back(new Components::TextButton(sInputManager));
		
			RECT buttonPos = { centerX - 96, centerY - 24, centerX + 96, centerY + 24 };
			mButtons[i]->Initialize(mDevice, buttonPos, btnCaptions[i]);
			centerY += 48 + padding;
		}

		RECT menuPos = { 100, 100, 200, 200 };
		mDragonAgeMenu = new Components::Menu(mDevice, sInputManager, menuPos);
	}

	MenuState::~MenuState() throw()
	{
		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			SafeDelete(mButtons.at(i));
		}
	}

	void MenuState::CreateBuffer(float width, float height)
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
	
	void MenuState::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Background.fx");
		
		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));
		inputLayout.push_back(InputLayoutElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
	}

	void MenuState::OnStatePushed()
	{
		for(int i = 0; i < MenuButton::Count; ++i)
		{
			sInputManager->AddMouseListener(mButtons[i]);
		}
	}

	void MenuState::OnStatePopped()
	{
		for(int i = 0; i < MenuButton::Count; ++i)
		{
			sInputManager->RemoveMouseListener(mButtons[i]);
		}
	}

	void MenuState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE] && !prevInput.Keyboard.keyIsPressed[VK_ESCAPE])
			QuitApplication();

		if(mButtons[MenuButton::Local]->GetAndResetClickStatus())
			ChangeState(C_STATE_LOCAL_LOBBY);
		if(mButtons[MenuButton::Network]->GetAndResetClickStatus())
			ChangeState(C_STATE_NETWORK_LOBBY);
		/*if(mButtons.at(MenuButton::Options)->GetAndResetClickStatus())
			ChangeState(C_STATE_IN_GAME);*/
		if(mButtons[MenuButton::Exit]->GetAndResetClickStatus())
			QuitApplication();

		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			mButtons.at(i)->Update(gameTime, currInput, prevInput);
		}
	}

	void MenuState::Draw()
	{
		mBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
		}

		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			mButtons[i]->Draw();
		}
	}
}
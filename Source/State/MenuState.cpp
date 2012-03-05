#include "MenuState.hpp"

namespace State
{
	MenuState::MenuState(StateID id, ID3D10Device* device) 
		: ApplicationState(id), mDevice(device),  mEffect(NULL), mBuffer(NULL), mComponents(NULL)
	{
		CreateBuffer((float)sViewport->GetWidth(), (float)sViewport->GetHeight());
		CreateEffect();
	}

	MenuState::~MenuState() throw()
	{
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

	void MenuState::CreateComponents()
	{
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "MenuState Group");

		ID3D10ShaderResourceView* texture;
		D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/titleScreen1422x800.png", NULL, NULL, 
											   &texture, NULL);
		mEffect->SetVariable("textureBG", texture);

		// Create menu buttons
		const std::string btnCaptions[] = { "Start Game", "Watch Replay", "Credits", "Exit Game" };
		const int width = 300;
		const int height = 75;
		const int padding = 0;
		LONG leftOffset = 0;
		LONG topOffset = (LONG)sViewport->GetHeight() - MenuButton::Count * (height + padding) + padding;
		POINT pos = { 0, topOffset };
		mMenuButtons = new Components::ClickMenu(mComponents, mDevice, pos, width, height); 

		for(int i = 0; i < MenuButton::Count; ++i)
		{
			mMenuButtons->AddMenuItem(btnCaptions[i]);
		}

		/*mButtons[1]->SetEnabled(false);
		mButtons[2]->SetEnabled(false);*/
	}

	void MenuState::OnStatePushed()
	{
		CreateComponents();
		mComponents->GiveFocus();
	}

	void MenuState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mButtons.clear();
	}

	void MenuState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if(currInput.Keyboard.keyIsPressed[VK_ESCAPE] && !prevInput.Keyboard.keyIsPressed[VK_ESCAPE])
			QuitApplication();

		//if(mButtons[MenuButton::StartGame]->GetAndResetClickStatus())
		//	ChangeState(C_STATE_CREATE_GAME);
		///*if(mButtons[MenuButton::WatchReplay]->GetAndResetClickStatus())
		//	ChangeState(C_STATE_NETWORK_LOBBY);*/
		///*if(mButtons.at(MenuButton::Options)->GetAndResetClickStatus())
		//	ChangeState(C_STATE_IN_GAME);*/
		//if(mButtons[MenuButton::Exit]->GetAndResetClickStatus())
		//	QuitApplication();

		//mComponents->Update(gameTime, currInput, prevInput);
	}

	void MenuState::Draw()
	{
		mBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
		}

		mComponents->Draw();
	}
}
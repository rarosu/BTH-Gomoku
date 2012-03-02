#include "LobbyState.hpp"
#include "Label.hpp"

namespace State
{
	LobbyState::LobbyState(StateID id, ID3D10Device* device)
		: ApplicationState(id),
		  mDevice(device), mComponents(NULL)
	{
		CreateBuffer((float)sViewport->GetWidth(), (float)sViewport->GetHeight());
		CreateEffect();
	}

	void LobbyState::CreateBuffer(float width, float height)
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
	
	void LobbyState::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Background.fx");
		
		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));
		inputLayout.push_back(InputLayoutElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);
	}

	void LobbyState::CreateComponents()
	{
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "LobbyState Group");

		ID3D10ShaderResourceView* texture;
		D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/marbleBG1422x800.png", NULL, NULL, 
											   &texture, NULL);
		mEffect->SetVariable("textureBG", texture);

		int leftOffset = 100;
		const std::string btnCaptions[] = { "Create", "Cancel" };
		LONG centerX = leftOffset + 96;
		LONG centerY = (LONG)sViewport->GetHeight() / 2;
		const int padding = 30;

		for(int i = 0; i < 2; ++i)
		{
			mButtons.push_back(new Components::TextButton(mComponents));
		
			RECT buttonPos = { centerX - 96, centerY - 24, centerX + 96, centerY + 24 };
			mButtons[i]->Initialize(mDevice, buttonPos, btnCaptions[i]);
			centerX += 192 + padding;
		}
	}

	void LobbyState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if(mButtons[LobbyButton::Create]->GetAndResetClickStatus())
			ChangeState(C_STATE_LOBBY);
		if(mButtons[LobbyButton::Cancel]->GetAndResetClickStatus())
			ChangeState(C_STATE_MENU);

		mComponents->Update(gameTime, currInput, prevInput);
	}

	void LobbyState::Draw()
	{
		mBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
		}

		mComponents->Draw();
	}

	void LobbyState::OnStatePushed()
	{
		CreateComponents();
		mComponents->GiveFocus();
	}

	void LobbyState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mButtons.clear();
	}
}
#include "MenuState.hpp"

namespace State
{
	MenuState::MenuState(StateID id, ID3D10Device* device, int width, int height) 
		: ApplicationState(id), mDevice(device),  mEffect(NULL), mBuffer(NULL)
	{
		CreateBuffer((float)width, (float)height);
		CreateEffect();

		ID3D10ShaderResourceView* texture;
		D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/titleScreenBig.png", NULL, NULL, 
											   &texture, NULL);
		mEffect->SetResourceVariable("textureBG", texture);

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

		mBuffer = new Buffer();
		BufferInformation bufferDesc;

		bufferDesc.type						= VertexBuffer;
		bufferDesc.usage					= Buffer_Default;
		bufferDesc.numberOfElements			= numVertices;
		bufferDesc.firstElementPointer		= vertices;
		bufferDesc.elementSize				= sizeof(bgVertex);
		bufferDesc.topology					= D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		mBuffer->Initialize(mDevice, bufferDesc);
	}
	
	void MenuState::CreateEffect()
	{
		// Create an array describing each of the elements of the vertex that are inputs to the vertex shader.
		D3D10_INPUT_ELEMENT_DESC vertexDesc[] = 
		{
			{ "POSITION",					// Semantic name, must be same as the vertex shader input semantic name
			  0,							// Semantic index, if one semantic name exists for more than one element
			  DXGI_FORMAT_R32G32_FLOAT,		// Format of the element, R32G32_FLOAT is a 32-bit 2D float vector
			  0,							// Input slot, of the 0-15 slots, through wich to send vertex data
			  0,							// AlignedByteOffset, bytes from start of the vertex to this component
			  D3D10_INPUT_PER_VERTEX_DATA,	// Input data class for this input slot
			  0 }, 							// 0 when slot input data class is D3D10_INPUT_PER_VERTEX_DATA
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 }
		};

		mEffect = new Effect();
		mEffect->Initialize(mDevice, "Resources/Effects/Background.fx", vertexDesc,
			sizeof(vertexDesc) / sizeof(D3D10_INPUT_ELEMENT_DESC));
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
		mBuffer->MakeActive();
		mEffect->MakeActive();

		for(UINT p = 0; p < mEffect->GetNumberOfPasses(); ++p)
		{
			mEffect->ApplyTechniquePass(p);
			mDevice->Draw(mBuffer->GetNumberOfElements(), 0);
		}

		for(UINT i = 0; i < mButtons.size(); ++i)
		{
			mButtons[i]->Draw();
		}
	}
}
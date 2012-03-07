#include "JoinGameState.hpp"
#include "Label.hpp"
#include "ClientSession.hpp"
#include <sstream>

namespace State
{
	JoinGameState::JoinGameState(StateID id, ID3D10Device* device)
		: ApplicationState(id), 
		  mDevice(device), 
		  mDefaultFont(NULL),
		  mBuffer(NULL),
		  mEffect(NULL),
		  mComponents(NULL),
		  mNameField(NULL),
		  mIPAddressField(NULL),
		  mPortField(NULL),
		  mJoinButton(NULL),
		  mCancelButton(NULL)
	{
		mDefaultFont = new GameFont(mDevice,  "Segoe Print", 48);
		CreateBuffer();
		CreateEffect();
	}

	JoinGameState::~JoinGameState() throw()
	{
		SafeDelete(mDefaultFont);
		SafeDelete(mBuffer);
		SafeDelete(mEffect);
	}

	void JoinGameState::OnStatePushed()
	{
		CreateComponents();
	}

	void JoinGameState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);

		mComponents = NULL;
		mNameField = NULL;
		mIPAddressField = NULL;
		mPortField = NULL;
		mJoinButton = NULL;
		mCancelButton = NULL;
	}

	void JoinGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if (mCancelButton->GetAndResetClickStatus())
		{
			ChangeState(C_STATE_MENU);
			return;
		}

		// Basic check to see if the name is valid (non-empty)
		if (mNameField->Empty())
		{
			mJoinButton->SetEnabled(false);
			return;
		}

		// Basic check to see if the IP/Hostname is valid (non-empty)
		if (mIPAddressField->Empty())
		{
			mJoinButton->SetEnabled(false);
			return;
		}

		// Basic check to see if the port is valid
		std::stringstream s;
		unsigned short port = 0;
		if (!(s >> port))
		{
			mJoinButton->SetEnabled(false);
			return;
		}

		// All is fine (so far)! Allow creation of client.
		mJoinButton->SetEnabled(true);
		
		if (mJoinButton->GetAndResetClickStatus())
		{
			// TODO: Create client and attempt to connect (report errors some way)
			//Network::Client* client = new Network::Client(
			// TODO: Send Join message and receive Accept message.
		}
	}

	void JoinGameState::Draw()
	{
		mBuffer->Bind();
		for (unsigned int p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
		}
	}

	void JoinGameState::CreateComponents()
	{
		// Define sizes
		const int C_OFFSET_LEFT = 100;
		const int C_OFFSET_TOP = 430;
		const int C_BUTTON_WIDTH = 192;
		const int C_BUTTON_HEIGHT = mDefaultFont->GetSize();
		const int C_INPUT_FIELD_WIDTH = 192;
		const int C_INPUT_FIELD_HEIGHT = mDefaultFont->GetSize();
		const int C_LABEL_WIDTH = 220;
		const int C_LABEL_HEIGHT = mDefaultFont->GetSize();
		RECT r;



		// Create all the components and put them in the right place
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "CreateGameState Group");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "Name:", r);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mNameField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mNameField->SetText("Player");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 2;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "IP/Hostname:", r);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mIPAddressField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mIPAddressField->SetText("127.0.0.1");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 4;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "Port:", r);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mPortField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mPortField->SetText("6666");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_BUTTON_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 6;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mJoinButton = new Components::TextButton(mComponents);
		mJoinButton->Initialize(mDevice, r, "Join Game");

		r.left = C_OFFSET_LEFT + C_BUTTON_WIDTH * 2;
		r.right = r.left + C_BUTTON_WIDTH;
		mCancelButton = new Components::TextButton(mComponents);
		mCancelButton->Initialize(mDevice, r, "Cancel");



		// Set the initial focus, and focus this component group
		mNameField->SetFocus();
		mComponents->SetFocus();
	}

	void JoinGameState::CreateBuffer()
	{
		bgVertex vertices[4];

		vertices[0].position = D3DXVECTOR2(-1, 1);
		vertices[0].uv = D3DXVECTOR2(0, 0);
		vertices[1].position = D3DXVECTOR2(1, 1);
		vertices[1].uv = D3DXVECTOR2(1, 0);
		vertices[2].position = D3DXVECTOR2(-1, -1);
		vertices[2].uv = D3DXVECTOR2(0, 1);
		vertices[3].position = D3DXVECTOR2(1, -1);
		vertices[3].uv = D3DXVECTOR2(1, 1);

		VertexBuffer::Data bufferDesc;
		bufferDesc.mTopology =				Topology::TriangleStrip;
		bufferDesc.mUsage =					Usage::Default;
		bufferDesc.mElementCount =			4;
		bufferDesc.mElementSize =			sizeof(bgVertex);
		bufferDesc.mFirstElementPointer =	vertices;

		mBuffer = new VertexBuffer(mDevice);
		mBuffer->SetData(bufferDesc, NULL);
	}

	void JoinGameState::CreateEffect()
	{
		mEffect = new Effect(mDevice, "Resources/Effects/Background.fx");
		
		InputLayoutVector inputLayout;
		inputLayout.push_back(InputLayoutElement("POSITION", DXGI_FORMAT_R32G32_FLOAT));
		inputLayout.push_back(InputLayoutElement("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT));

		mEffect->GetTechniqueByIndex(0).GetPassByIndex(0).SetInputLayout(inputLayout);

		ID3D10ShaderResourceView* texture;
		D3DX10CreateShaderResourceViewFromFile(mDevice, "Resources/Textures/marbleBG1422x800.png", NULL, NULL, &texture, NULL);
		mEffect->SetVariable("textureBG", texture);
	}
}
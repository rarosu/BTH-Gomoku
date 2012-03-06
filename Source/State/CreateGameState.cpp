#include "CreateGameState.hpp"
#include "ServerSession.hpp"

namespace State
{
	CreateGameState::CreateGameState(StateID id, ID3D10Device* device, LobbyState* lobbyState)
		: ApplicationState(id),
		  mDevice(device), 
		  mComponents(NULL), 
		  mDefaultFont(NULL),
		  mLobbyState(lobbyState),
		  mIFName(NULL),
		  mIFPort(NULL),
		  mBtnCreate(NULL),
		  mBtnCancel(NULL)
	{
		CreateBuffer((float)sViewport->GetWidth(), (float)sViewport->GetHeight());
		CreateEffect();

		mDefaultFont = new GameFont(mDevice, "Segoe Print", 48);
	}

	CreateGameState::~CreateGameState() throw()
	{
		SafeDelete(mDefaultFont);
		SafeDelete(mComponents);
		SafeDelete(mBuffer);
		SafeDelete(mEffect);
	}

	void CreateGameState::CreateBuffer(float width, float height)
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
	
	void CreateGameState::CreateEffect()
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

	void CreateGameState::CreateComponents()
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
		mIFName = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mIFName->SetText("Admin");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_LABEL_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 2;
		r.bottom = r.top + C_LABEL_HEIGHT;
		new Components::Label(mDevice, mComponents, "Port:", r);

		r.left = C_OFFSET_LEFT + C_LABEL_WIDTH;
		r.right = r.left + C_INPUT_FIELD_WIDTH;
		r.bottom = r.top + C_INPUT_FIELD_HEIGHT;
		mIFPort = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);
		mIFPort->SetText("6666");

		r.left = C_OFFSET_LEFT;
		r.right = r.left + C_BUTTON_WIDTH;
		r.top = C_OFFSET_TOP + C_LABEL_HEIGHT * 4;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mBtnCreate = new Components::TextButton(mComponents);
		mBtnCreate->Initialize(mDevice, r, "Create Game");

		r.left = C_OFFSET_LEFT + C_BUTTON_WIDTH * 2;
		r.right = r.left + C_BUTTON_WIDTH;
		mBtnCancel = new Components::TextButton(mComponents);
		mBtnCancel->Initialize(mDevice, r, "Cancel");



		// Set the initial focus, and focus this component group
		mComponents->SetFocus(mIFPort);
		mComponents->GiveFocus();	
	}

	void CreateGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{	
		if (mBtnCancel->GetAndResetClickStatus())
		{
			ChangeState(C_STATE_MENU);
			return;
		}


		Logic::ServerParameters args;
		std::stringstream s;

		// Basic check to see if the port is valid
		s.str(mIFPort->GetText());
		if (!(s >> args.mPort))
		{
			mBtnCreate->SetEnabled(false);
			return;
		}

		// Basic check to see if the name is valid (non-empty)
		args.mAdminName = mIFName->GetText();
		if (args.mAdminName.empty())
		{
			mBtnCreate->SetEnabled(false);
			return;
		}

		// All is fine (so far)! Allow creation of server.
		mBtnCreate->SetEnabled(true);

		if (mBtnCreate->GetAndResetClickStatus())
		{
			args.mRuleset = new Logic::StandardRuleset();

			// TODO: Attempt to create a server here, and report failure in some nifty way.
			
		}
	}

	void CreateGameState::Draw()
	{
		mBuffer->Bind();
		for(UINT p = 0; p < mEffect->GetTechniqueByIndex(0).GetPassCount(); ++p)
		{
			mEffect->GetTechniqueByIndex(0).GetPassByIndex(p).Apply(mDevice);
			mBuffer->Draw();
		}
	}

	void CreateGameState::OnStatePushed()
	{
		CreateComponents();
	}

	void CreateGameState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);

		mComponents = NULL;
		mIFName = NULL;
		mIFPort = NULL;
		mBtnCreate = NULL;
		mBtnCancel = NULL;
	}
}
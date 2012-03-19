#include "CreditsState.hpp"
#include "Label.hpp"

namespace State
{
	CreditsState::CreditsState(StateID id, ID3D10Device* device)
		: ApplicationState(id), 
		  mDevice(device),
		  mDefaultFont(NULL),
		  mBackground(NULL),
		  mBtnBack(NULL)
	{
		mDefaultFont = new GameFont(mDevice,  "Segoe Print", 48);
		mBackground = new Sprite(mDevice, sViewport, "marbleBG1422x800.png", sViewport->GetWidth(), sViewport->GetHeight());
	}

	CreditsState::~CreditsState() throw()
	{
		SafeDelete(mDefaultFont);
		SafeDelete(mBackground);
	}

	void CreditsState::OnStatePushed()
	{
		CreateComponents();
	}

	void CreditsState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);

		mComponents = NULL;
		mBtnBack = NULL;
	}

	void CreditsState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if(mBtnBack->GetAndResetClickStatus())
			ChangeState(C_STATE_MENU);
	}

	void CreditsState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0, 0));
	}

	void CreditsState::CreateComponents()
	{
		// Define sizes
		const int C_BUTTON_WIDTH = 192;
		const int C_BUTTON_LEFT = (sViewport->GetWidth() - C_BUTTON_WIDTH) / 2;
		const int C_BUTTON_HEIGHT = mDefaultFont->GetSize();
		RECT r;

		// Create all the components and put them in the right place
		RECT compPos = { 0, 0, 0, 0 };
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "JoinGameState Group", compPos);

		r.left = 100;
		r.right = sViewport->GetWidth() - r.left;
		r.top = 30;
		r.bottom = 150;
		new Components::Label(mDevice, mComponents, "CREDITS", r);

		std::string credits;
		credits = "Kim Restad\nThomas Sievert\nLars Woxberg";
		r.left = 200;
		r.right = sViewport->GetWidth() - r.left;
		r.top = 200;
		r.bottom = sViewport->GetHeight() - 100;
		new Components::Label(mDevice, mComponents, credits, r, 50, GameFont::Center, GameFont::Top);


		r.left = C_BUTTON_LEFT;
		r.right = r.left + C_BUTTON_WIDTH;
		r.top = sViewport->GetHeight() - 100;
		r.bottom = r.top + C_BUTTON_HEIGHT;
		mBtnBack = new Components::TextButton(mComponents, r);
		mBtnBack->Initialize(mDevice, "Back");
		
		mComponents->SetFocus();
	}
}
#include "CreateAGameState.hpp"

namespace State
{
	CreateAGameState::CreateAGameState(StateID id, ID3D10Device* device)
		: ApplicationState(id),
		  mDevice(device), mComponents(NULL), mDefaultFont(NULL)
	{
	}

	void CreateAGameState::CreateComponents()
	{
		mComponents = new Components::ComponentGroup(sRootComponentGroup, "CreateAGameState Group");

		mBackground = new Sprite(mDevice, sViewport,  "marbleBG1422x800.png", 
							 sViewport->GetWidth(), sViewport->GetHeight());

		int leftOffset = 100;
		const std::string btnCaptions[] = { "Create", "Cancel" };
		LONG topOffset = 430;
		const int width = 192;
		const int padding = 30;

		for(int i = 0; i < 2; ++i)
		{
			mButtons.push_back(new Components::TextButton(mComponents));
		
			RECT buttonPos = { leftOffset + (i * (width + padding)), topOffset, 
							   leftOffset + (i * (width + padding)) + width, topOffset + 48 };
			mButtons[i]->Initialize(mDevice, buttonPos, btnCaptions[i]);
		}

		Components::TextButton* tempBtn = new Components::TextButton(mComponents);
		RECT buttonPos = { leftOffset, 200, leftOffset + 414, 248 };
		tempBtn->Initialize(mDevice, buttonPos, "1 v 1 Normal Game");
		mButtons.push_back(tempBtn);

		RECT labelPos = { leftOffset, 260, leftOffset + 90, 308 };
		Components::Label* label = new Components::Label(mDevice, mComponents, "Port:", labelPos);
		// Set default font
		mDefaultFont = new GameFont(mDevice,  "Segoe Print", (labelPos.bottom - labelPos.top));
		RECT ifPos = { labelPos.right + 10, labelPos.top, buttonPos.right, labelPos.bottom };
		Components::InputField* input = new Components::InputField(mDevice, mComponents, NULL, ifPos, mDefaultFont);
		mComponents->SetFocusedComponent(input);

		RECT labelNPos = { leftOffset, 320, leftOffset + 90, 368 };
		mLblName = new Components::Label(mDevice, mComponents, "Name:", labelNPos);
		RECT ifNPos = { labelNPos.right + 10, labelNPos.top, buttonPos.right, labelNPos.bottom };
		mIFName = new Components::InputField(mDevice, mComponents, NULL, ifNPos, mDefaultFont);

		RECT labelTPos = { 0, 50, 600, 150 };
		Components::Label* labelTitle = new Components::Label(mDevice, mComponents, "CREATE A GAME", labelTPos);
	}

	void CreateAGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{		
		if(mButtons[CAGButton::Create]->GetAndResetClickStatus())
			ChangeState(C_STATE_LOBBY);
		if(mButtons[CAGButton::Cancel]->GetAndResetClickStatus())
			ChangeState(C_STATE_MENU);

		mComponents->Update(gameTime, currInput, prevInput);
	}

	void CreateAGameState::Draw()
	{
		mBackground->Draw(D3DXVECTOR2(0, 0));
		mComponents->Draw();
	}

	void CreateAGameState::OnStatePushed()
	{
		CreateComponents();
		mComponents->SetFocus();
	}

	void CreateAGameState::OnStatePopped()
	{
		sRootComponentGroup->RemoveComponent(mComponents);
		mComponents = NULL;
		mButtons.clear();
		mDefaultFont = NULL;
		SafeDelete(mBackground);
	}
}
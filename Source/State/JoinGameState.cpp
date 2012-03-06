#include "JoinGameState.hpp"

namespace State
{
	JoinGameState::JoinGameState(StateID id, ID3D10Device* device)
		: ApplicationState(id), mDevice(device), mDefaultFont(NULL)
	{
		mDefaultFont = new GameFont(mDevice,  "Segoe Print", 40);
	}

	JoinGameState::~JoinGameState() throw()
	{
		SafeDelete(mComponents);
		SafeDelete(mDefaultFont);
	}

	void JoinGameState::OnStatePushed()
	{
		CreateComponents();
	}

	void JoinGameState::OnStatePopped()
	{
		SafeDelete(mComponents);
		mNameField = NULL;
		mIPAddressField = NULL;
		mPortField = NULL;
		mJoinButton = NULL;
		mCancelButton = NULL;
	}

	void JoinGameState::Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime)
	{
		if (mJoinButton->GetAndResetClickStatus())
		{
			ChangeState(C_STATE_IN_GAME);
		}
		if (mCancelButton->GetAndResetClickStatus())
		{
			ChangeState(C_STATE_MENU);
		}
	}

	void JoinGameState::Draw()
	{
		
	}

	void JoinGameState::CreateComponents()
	{
		RECT r;
		const int K_INPUT_FIELD_WIDTH = 90;
		const int K_INPUT_FIELD_HEIGHT = 40;
		const int K_BUTTON_WIDTH = 90;
		const int K_BUTTON_HEIGHT = 40;

		mComponents = new Components::ComponentGroup(sRootComponentGroup, "JoinGameState root");
		
		r.left = 100;
		r.right = r.left + K_INPUT_FIELD_WIDTH;
		r.top = 320;
		r.bottom = r.top + K_INPUT_FIELD_HEIGHT;
		mNameField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);

		r.left = 100;
		r.right = r.left + K_INPUT_FIELD_WIDTH;
		r.top = 320 + 2 * K_INPUT_FIELD_HEIGHT;
		r.bottom = r.top + K_INPUT_FIELD_HEIGHT;
		mIPAddressField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);

		r.left = 100;
		r.right = r.left + K_INPUT_FIELD_WIDTH;
		r.top = 320 + 4 * K_INPUT_FIELD_HEIGHT;
		r.bottom = r.top + K_INPUT_FIELD_HEIGHT;
		mPortField = new Components::InputField(mDevice, mComponents, NULL, r, mDefaultFont);

		r.left = 100;
		r.right = r.left + K_BUTTON_WIDTH;
		r.top = 320 + 6 * K_INPUT_FIELD_HEIGHT;
		r.bottom = r.top + K_BUTTON_HEIGHT;
		mJoinButton = new Components::TextButton(mComponents);
		mJoinButton->Initialize(mDevice, r, "JOIN");

		r.left = 100 + 2 * K_BUTTON_WIDTH;
		r.right = r.left + K_BUTTON_WIDTH;
		r.top = 320 + 6 * K_INPUT_FIELD_HEIGHT;
		r.bottom = r.top + K_BUTTON_HEIGHT;
		mCancelButton = new Components::TextButton(mComponents);
		mCancelButton->Initialize(mDevice, r, "CANCEL");

		mComponents->SetFocus(mNameField);
		mComponents->GiveFocus();
		
	}
}
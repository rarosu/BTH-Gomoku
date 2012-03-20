#ifndef CREATE_GAME_STATE_HPP
#define CREATE_GAME_STATE_HPP

#include "ApplicationState.hpp"
#include "TextButton.hpp"
#include "GameFont.hpp"
#include "Label.hpp"
#include "InputField.hpp"
#include "GameFont.hpp"
#include "Sprite.hpp"
#include "ServerLobbyState.hpp"

namespace State
{
	class CreateGameState : public ApplicationState
	{
	public:
		CreateGameState(StateID id, ID3D10Device* device, ServerLobbyState* serverLobbyState);
		~CreateGameState() throw();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();
	private:
		struct bgVertex
		{
			D3DXVECTOR2		position;
			D3DXVECTOR2		uv;
		};

		ID3D10Device*							mDevice;
		Components::ComponentGroup*				mComponents;
		Sprite*									mBackground;
		GameFont*								mDefaultFont;
		Components::Label*						mLblName;		
		ServerLobbyState*						mServerLobbyState;

		Components::InputField*					mIFName;
		Components::InputField*					mIFPort;
		Components::TextButton*					mBtnCreate;
		Components::TextButton*					mBtnCancel;
		Components::ToggleButton*				mGameType;
		Components::ToggleButton*				mPlayerType;

		void CreateComponents();
		void SaveSetupInfo();
		void LoadSetupInfo();
	};
}
#endif
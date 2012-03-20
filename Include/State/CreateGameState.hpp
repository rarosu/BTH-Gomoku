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

		struct GameType
		{
			enum Type { Normal, Crazy };
			enum Players { Players1v1, Players2v2 };

			Type		mType;
			Players		mPlayers;

			GameType() : mType(Type::Normal), mPlayers(Players::Players1v1) {}
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
		Components::ClickMenu*					mGameType;
		Components::ClickMenu*					mPlayerType;

		GameType								mChosenGame;
		bool									mJustChosenP; // DEBUG
		bool									mJustChosenG; // DEBUG

		void CreateComponents();
	};
}
#endif
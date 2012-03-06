#ifndef CREATE_GAME_STATE_HPP
#define CREATE_GAME_STATE_HPP

#include "ApplicationState.hpp"
#include "TextButton.hpp"
#include "Effect.hpp"
#include "Buffer.hpp"
#include "GameFont.hpp"
#include "Label.hpp"
#include "InputField.hpp"
#include "GameFont.hpp"
#include "LobbyState.hpp"

namespace State
{
	class CreateGameState : public ApplicationState
	{
	public:
		CreateGameState(StateID id, ID3D10Device* device, LobbyState* lobbyState);
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
		Effect*									mEffect;
		VertexBuffer*							mBuffer;
		GameFont*								mDefaultFont;		

		LobbyState*								mLobbyState;

		Components::ComponentGroup*				mComponents;
		Components::InputField*					mIFName;
		Components::InputField*					mIFPort;
		Components::TextButton*					mBtnCreate;
		Components::TextButton*					mBtnCancel;

		void CreateComponents();
		void CreateBuffer(float width, float height);
		void CreateEffect();
	};
}
#endif
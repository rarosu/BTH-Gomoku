#ifndef CREATE_A_GAME_STATE_HPP
#define CREATE_A_GAME_STATE_HPP

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
	namespace CAGButton
	{
		enum Button
		{
			Create,
			Cancel,
			Count
		};
	}

	class CreateAGameState : public ApplicationState
	{
	public:
		CreateAGameState(StateID id, ID3D10Device* device, LobbyState* lobbyState);

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
		Components::ComponentGroup*				mComponents;
		std::vector<Components::TextButton*>	mButtons;

		GameFont*								mDefaultFont;
		Components::Label*						mLblName;
		Components::InputField*					mIFName;

		LobbyState*								mLobbyState;

		void CreateComponents();
		void CreateBuffer(float width, float height);
		void CreateEffect();
	};
}
#endif
#ifndef LOBBY_STATE_HPP
#define LOBBY_STATE_HPP

#include "ApplicationState.hpp"
#include "TextButton.hpp"
#include "Label.hpp"
#include "Effect.hpp"
#include "Buffer.hpp"

namespace State
{
	namespace LobbyButton
	{
		enum Button
		{
			Team1Player1,
			Team1Player2,
			Team2Player1,
			Team2Player2,
			StartGame,
			Cancel,
			Count
		};
	}

	class LobbyState : public ApplicationState
	{
	public:
		LobbyState(StateID id, ID3D10Device* device);

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
		std::vector<Components::Label*>			mPlayerLabels;

		void CreateComponents();
		void CreateBuffer(float width, float height);
		void CreateEffect();
	};
}
#endif
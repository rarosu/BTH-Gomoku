#ifndef JOIN_A_GAME_STATE_HPP
#define JOIN_A_GAME_STATE_HPP

#include "ApplicationState.hpp"
#include "LobbyState.hpp"

namespace State
{
	class JoinAGameState : public ApplicationState
	{
	public:
		JoinAGameState(StateID id, ID3D10Device* device, LobbyState* lobbyState);
		~JoinAGameState() throw();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();
	private:
		ID3D10Device*							mDevice;
		Effect*									mEffect;
		VertexBuffer*							mBuffer;

		Components::ComponentGroup*				mComponents;
		std::vector<Components::TextButton*>	mButtons;

		LobbyState*								mLobbyState;
	};
}

#endif
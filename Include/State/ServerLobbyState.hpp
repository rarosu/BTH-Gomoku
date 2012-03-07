#ifndef SERVER_LOBBY_STATE_HPP
#define SERVER_LOBBY_STATE_HPP

#include "ApplicationState.hpp"
#include "TextButton.hpp"
#include "Label.hpp"
#include "Effect.hpp"
#include "Buffer.hpp"
#include "ServerSession.hpp"
#include "Server.hpp"

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

	class ServerLobbyState : public ApplicationState
	{
	public:
		ServerLobbyState(StateID id, ID3D10Device* device);
		~ServerLobbyState() throw();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();

		void SetSessionArguments(Network::Server* server, const std::string& adminName, Logic::Ruleset* ruleset);
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

		Logic::ServerSession*					mSession;

		void CreateComponents();
		void CreateBuffer(float width, float height);
		void CreateEffect();
	};
}
#endif
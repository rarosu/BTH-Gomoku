#ifndef SERVER_LOBBY_STATE_HPP
#define SERVER_LOBBY_STATE_HPP

#include "ApplicationState.hpp"
#include "Console.hpp"
#include "TextButton.hpp"
#include "Label.hpp"
#include "Sprite.hpp"
#include "ServerSession.hpp"
#include "Server.hpp"

namespace State
{
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
		Sprite*									mBackground;

		Components::ComponentGroup*				mComponents;
		std::vector<Components::Label*>			mPlayerLabels;
		Components::TextButton*					mStartButton;
		Components::TextButton*					mCancelButton;
		Components::Console*					mChat;

		Logic::ServerSession*					mSession;

		void CreateComponents();
	};
}
#endif
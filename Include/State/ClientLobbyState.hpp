#ifndef CLIENT_LOBBY_STATE_HPP
#define CLIENT_LOBBY_STATE_HPP

#include "ApplicationState.hpp"
#include "Sprite.hpp"
#include "ClientSession.hpp"
#include "ClientGameState.hpp"
#include "TextButton.hpp"
#include "Label.hpp"
#include "ChatConsole.hpp"

namespace State
{
	class ClientLobbyState : public ApplicationState, public Logic::ChatReceiver, public Logic::ClientNotificationInterface, public Components::ChatInputReceiver
	{
	public:
		ClientLobbyState(StateID id, ID3D10Device* device, State::ClientGameState* clientGameState);
		~ClientLobbyState() throw();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();

		void SetSessionArguments(Network::Client* client, int playerCount, int selfID, const std::string& name);

		void ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message);
		void ReceiveChatMessage(const std::string& message, unsigned int sourceID);

		void GameStarted();
	private:
		struct bgVertex
		{
			D3DXVECTOR2		position;
			D3DXVECTOR2		uv;
		};

		ID3D10Device*							mDevice;
		Sprite*									mBackground;

		Components::ComponentGroup*				mComponents;
		Components::TextButton*					mCancelButton;
		Components::ChatConsole*				mChat;
		std::vector<Components::Label*>			mPlayerLabels;

		Logic::ClientSession*					mSession;

		State::ClientGameState*					mClientGameState;

		void CreateComponents();
	};
}
#endif
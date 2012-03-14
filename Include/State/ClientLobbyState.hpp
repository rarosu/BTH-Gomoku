#ifndef CLIENT_LOBBY_STATE_HPP
#define CLIENT_LOBBY_STATE_HPP

#include "ApplicationState.hpp"
#include "Sprite.hpp"
#include "ClientSession.hpp"
#include "TextButton.hpp"
#include "Label.hpp"
#include "Console.hpp"

namespace State
{
	class ClientLobbyState : public ApplicationState, public Logic::ChatReceiver, public Components::ConsoleInputReceiver
	{
	public:
		ClientLobbyState(StateID id, ID3D10Device* device);
		~ClientLobbyState() throw();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();

		void SetSessionArguments(Network::Client* client, int playerCount, int selfID, const std::string& name);

		void ConsoleInputEntered(const Components::Console* consoleInstance, const std::string& message);
		void ReceiveChatMessage(const std::string& message, unsigned int sourceID);
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
		Components::Console*					mChat;
		std::vector<Components::Label*>			mPlayerLabels;

		Logic::ClientSession*					mSession;

		void CreateComponents();
	};
}
#endif
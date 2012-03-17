#ifndef ABSTRACT_GAME_STATE_HPP
#define ABSTRACT_GAME_STATE_HPP

#include "ApplicationState.hpp"
#include "Scene.hpp"
#include "ComponentGroup.hpp"
#include "ChatConsole.hpp"
#include "Session.hpp"

namespace State
{
	class AbstractGameState : public ApplicationState, public Logic::ChatReceiver, public Components::ChatInputReceiver
	{
	public:
		AbstractGameState(StateID id, ID3D10Device* device);
		~AbstractGameState() throw();

		void OnStatePushed();
		void OnStatePopped();

		void OnResize();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message);
		void ReceiveChatMessage(const std::string& message, unsigned int sourceID);
	protected:
		void SetSession(Logic::Session* session);

		virtual void InitializeGame() {}
		virtual void EndGame() {}

		void SetChatName(const std::string& name);
	private:
		ID3D10Device* mDevice;
		Logic::Session* mSession;

		Components::ComponentGroup*	mComponents;
		Scene* mScene;
		Components::ChatConsole* mChat;

		void CreateComponents();
		float GetAspectRatio();
	};
}

#endif
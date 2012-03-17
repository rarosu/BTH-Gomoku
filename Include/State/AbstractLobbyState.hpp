#ifndef ABSTRACT_LOBBY_STATE_HPP
#define ABSTRACT_LOBBY_STATE_HPP

#include "ApplicationState.hpp"
#include "Session.hpp"

#include "ComponentGroup.hpp"
#include "ChatConsole.hpp"
#include "TextButton.hpp"
#include "Label.hpp"



namespace State
{
	class AbstractLobbyState : public ApplicationState, public Logic::ChatReceiver, public Components::ChatInputReceiver
	{
	public:
		AbstractLobbyState(StateID id, ID3D10Device* device);
		~AbstractLobbyState() throw();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();

		void ChatInputEntered(const Components::ChatConsole* consoleInstance, const std::string& message);
		void ReceiveChatMessage(const std::string& message, unsigned int sourceID);
	protected:
		static const int C_LABEL_WIDTH		= 150;
		static const int C_LABEL_HEIGHT		= 48;
		static const int C_LABEL_MARGIN		= 10;
		static const int C_BUTTON_YOFFSET	= 60;
		static const int C_BUTTON_WIDTH		= 150;
		static const int C_BUTTON_HEIGHT	= 48;
		static const int C_CHAT_HEIGHT		= 150;

		void SetSession(Logic::Session* session);

		virtual void AppendStatePopped() {}
		virtual void AppendComponents() {}
		virtual void AppendUpdate() {}



		ID3D10Device*							mDevice;

		Components::ComponentGroup*				mComponents;
		Components::TextButton*					mCancelButton;
		Components::ChatConsole*				mChat;
		std::vector<Components::Label*>			mPlayerLabels;
	private:
		Sprite*									mBackground;
		Logic::Session*							mSession;

		void CreateComponents();
	};
}

#endif
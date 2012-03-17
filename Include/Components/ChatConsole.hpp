#ifndef CHAT_CONSOLE_HPP
#define CHAT_CONSOLE_HPP

#include "Console.hpp"

namespace Components
{
	class ChatConsole;

	/**
		Defines the notifiee for input entered into a chat.
	*/
	class ChatInputReceiver
	{
	public:
		virtual void ChatInputEntered(const ChatConsole* consoleInstance, const std::string& message) = 0;
	};



	/**
		Defines a chat console
	*/
	class ChatConsole : public Console
	{
	public:
		ChatConsole(ID3D10Device* device, ComponentGroup* ownerGroup, const RECT& position, const D3DXCOLOR& background, 
					const std::string& name, UINT size = 100);

		/**
			Set and get the chat receiver.
		*/
		void SetChatReceiver(ChatInputReceiver* receiver);
		const ChatInputReceiver* GetChatReceiver() const;

		/**
			Set and get the owner's name. Changing the name will NOT change the chat log
			history.
		*/
		void SetName(const std::string& name);
		const std::string& GetName() const;

		/**
			Receive input from the input field, print it
			with the given name in the chatlog, and notify
			the chat receiver about the entered string.
		*/
		void ReceiveInput(std::string input);

		/**
			Add a string to the chatlog, without any
			notifications or modifications.
		*/
		void AddLine(const std::string& input);
	private:
		std::string mName;
		ChatInputReceiver* mChatReceiver;
	};
}

#endif
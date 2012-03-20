#include "ChatConsole.hpp"
#include <sstream>

namespace Components
{
	ChatConsole::ChatConsole(Logic::Session* session, ID3D10Device* device, ComponentGroup* ownerGroup, const RECT& position, const D3DXCOLOR& background, const std::string& name, UINT size)
		: Console(device, ownerGroup, position, background, size)
		, mSession(session)
		, mName(name)
		, mChatReceiver(NULL)
	{}

	void ChatConsole::SetChatReceiver(ChatInputReceiver* receiver)
	{
		mChatReceiver = receiver;
	}

	const ChatInputReceiver* ChatConsole::GetChatReceiver() const
	{
		return mChatReceiver;
	}

	void ChatConsole::SetName(const std::string& name)
	{
		mName = name;
	}

	const std::string& ChatConsole::GetName() const
	{
		return mName;
	}

	void ChatConsole::ReceiveInput(std::string input)
	{
		// Check for target
		Logic::PlayerID target = Logic::C_PLAYER_NONE;
		Network::Recipient::Recipient recipient = Network::Recipient::Broadcast;

		std::string msg = input;
		std::string targetName;
		if (input[0] == '/')
		{
			std::stringstream s(input.substr(1));
			std::getline(s, targetName, ' ');

			if (targetName == "Team")
			{
				recipient = Network::Recipient::Team;
			}
			else
			{
				target = mSession->GetPlayerByName(targetName);
				if (target != Logic::C_PLAYER_NONE)
				{
					recipient = Network::Recipient::Private;
				}
			}
		}

		if (recipient != Network::Recipient::Broadcast)
			msg = msg.substr(2 + targetName.size());

		if (mChatReceiver != NULL)
			mChatReceiver->ChatInputEntered(this, msg, target, recipient);

		AddOutgoingMessage(msg, recipient, target);
	}

	void ChatConsole::AddLine(const std::string& input)
	{
		Console::ReceiveInput(input);
	}

	void ChatConsole::AddOutgoingMessage(const std::string& message, Network::Recipient::Recipient recipient, Logic::PlayerID targetID)
	{
		std::string finalMessage = message;
		switch (recipient)
		{
			case Network::Recipient::Broadcast:
				finalMessage = mName + ": " + finalMessage;
			break;

			case Network::Recipient::Private:
				finalMessage = mName + " to " + mSession->GetPlayerName(targetID) + ": " + finalMessage;
			break;

			case Network::Recipient::Team:
				finalMessage = mName + " (Team): " + finalMessage;
			break;
		}

		Console::ReceiveInput(finalMessage);
	}

	void ChatConsole::AddIncomingMessage(const std::string& message, Network::Recipient::Recipient recipient, Logic::PlayerID sourceID)
	{
		std::string finalMessage = message;
		switch (recipient)
		{
			case Network::Recipient::Broadcast:
				finalMessage = mSession->GetPlayerName(sourceID) + ": " + finalMessage;
			break;

			case Network::Recipient::Private:
				finalMessage = mSession->GetPlayerName(sourceID) + " (Private): " + finalMessage;
			break;

			case Network::Recipient::Team:
				finalMessage = mSession->GetPlayerName(sourceID) + " (Team): " + finalMessage;
			break;
		}

		Console::ReceiveInput(finalMessage);
	}
}
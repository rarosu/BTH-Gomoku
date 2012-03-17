#include "ChatConsole.hpp"

namespace Components
{
	ChatConsole::ChatConsole(ID3D10Device* device, ComponentGroup* ownerGroup, const RECT& position, const D3DXCOLOR& background, const std::string& name, UINT size)
		: Console(device, ownerGroup, position, background, size)
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
		if (mChatReceiver != NULL)
			mChatReceiver->ChatInputEntered(this, input);
		
		input = mName + ": " + input;
		Console::ReceiveInput(input);
	}

	void ChatConsole::AddLine(const std::string& input)
	{
		Console::ReceiveInput(input);
	}
}
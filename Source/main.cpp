/*
#include "D3DApplication.hpp"
#include "Game.hpp"

// ----------------------------------------------------- METHODS ------------------------------------------------------ //
// The main function, where it all starts. Initializes the window and runs the function
int WINAPI WinMain(HINSTANCE applicationInstance, HINSTANCE prevInstance, PSTR cmdLineArgs, int showSetting)
{
	Game game(applicationInstance);
	return game.Run();
}
*/

#include <iostream>
#include <string>
#include "Server.hpp"
#include "Client.hpp"
#include "ChatMessage.hpp"

using namespace std;

void StartServer();
void StartClient();

int main()
{
	std::string input;
	cout << "1. Server (default)" << endl;
	cout << "2. Client" << endl;
	cout << "> ";
	getline(cin, input);

	if (input != "q")
	{
		if (input == "2")
			StartClient();
		else
			StartServer();
	}

	return 0;
}

void StartServer()
{
	Network::Server server(4);

	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		server.Update();

		Network::Message* message;
		while ( (message = server.PopMessage()) != NULL )
		{
			if (message->ID() == Network::C_MESSAGE_CHAT)
			{
				// Just echo the message to all clients
				Network::ChatMessage* chat = static_cast<Network::ChatMessage*>(message);
				cout << "Chat (" << chat->mSourceID << ") > " << chat->mMessage << endl;
				server.Send(*chat);
			}
		}

		// TODO: Have to know when players connect, in order to send Accept messages to them
		// and add them as players in playerlist

		// TODO: Have to know when players disconnect, in order to gracefully handle it and notify
		// the rest of the players
	}
}

void StartClient()
{
	Network::Client client;

	// TODO: Have to know whether we are connected or not
	// TODO: Specify timeout?

	bool writeMessage = false;
	std::string message;
	while (true)
	{
		// Check async keys
		if (GetAsyncKeyState(VK_ESCAPE))
			break;
		if (GetAsyncKeyState(VK_F1))
			writeMessage = true;

		// Check if the user wants to write something
		if (writeMessage)
		{
			cout << "Write message: ";
			getline(cin, message);
			cout << "Sending message: " << message << endl;

			Network::ChatMessage m;
			m.mMessage = message;
			client.Send(m);

			// TODO: Sending and parsing chat messages is messed up

			writeMessage = false;
		}

		// Receive and parse messages
		int receivedMessageCount = client.Update();
		for (int i = 0; i < receivedMessageCount; ++i)
		{
			// TODO: What happens if we pop a message that doesn't exist?
			Network::Message* message = client.PopMessage();
			switch (message->ID())
			{
				case Network::C_MESSAGE_CHAT:
				{
					Network::ChatMessage* chat = static_cast<Network::ChatMessage*>(message);
					cout << "Chat (" << chat->mSourceID << ") > " << chat->mMessage << endl;
				} break;

				default:
					cout << "Unhandled message received: don't mind me" << endl;
			}
		}
	}
}

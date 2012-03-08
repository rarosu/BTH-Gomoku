#include "ClientSession.hpp"

namespace Logic
{
	ClientSession::ClientSession(Network::Client* client, const std::string& playerName, unsigned int playerCount, unsigned int selfID)
		: mClient(client)
	{
		mPlayers.resize(playerCount);
		mPlayers[selfID].SetName(playerName);
		// TODO: Set mPlayers[selfID] to LocalPlayer
	}

	ClientSession::~ClientSession()
	{
		SafeDelete(mClient);
	}

	void ClientSession::Update()
	{
		mClient->Update();
	}

	unsigned int ClientSession::GetPlayerCount() const
	{
		return mPlayers.size();
	}

	const std::string& ClientSession::GetPlayerName(Network::Slot slot) const
	{
		return mPlayers[slot].GetName();
	}
}
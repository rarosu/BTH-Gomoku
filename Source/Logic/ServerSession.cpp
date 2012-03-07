#include "ServerSession.hpp"
#include "MessageInclude.hpp"
#include <sstream>


using namespace Network;



namespace Logic
{
	const float ServerSession::C_TIMEOUT = 1.0f;

	ServerSession::ServerSession(Network::Server* server, const std::string& adminName, Ruleset* ruleset) 
		: mServer(server)
		, mRuleset(ruleset)
	{
		mServer->SetEventInterface(this);
		mPlayers.resize(mRuleset->GetPlayerCount());
		
		// TODO: Set admin to local controller
		mPlayers[0].SetController(NULL);
		mPlayers[0].SetName(adminName);
	}

	ServerSession::~ServerSession() throw()
	{
		SafeDelete(mServer);
		SafeDelete(mRuleset);
	}

	unsigned short ServerSession::GetPort() const
	{
		return mServer->GetPort();
	}

	const Ruleset* ServerSession::GetRuleset() const
	{
		return mRuleset;
	}

	void ServerSession::Update(const GameTime& gameTime)
	{
		float dt = gameTime.GetTimeSinceLastTick().Seconds;
		for (TimeoutMap::iterator it = mTimeoutCounter.begin(); it != mTimeoutCounter.end(); ++it)
		{
			it->second -= dt;
			if (it->second <= 0.0f)
			{
				mServer->DisconnectClient(it->first);
				
				mTimeoutCounter.erase(it);
				it--;
			}
		}

		mServer->Update();

		Message* message;
		while ((message = mServer->PopMessage()) != NULL)
		{
			switch (message->ID())
			{
			case Network::C_MESSAGE_ACCEPT:
				message = static_cast<Network::AcceptMessage*>(message);
				break;
			case Network::C_MESSAGE_ADD_PLAYER:
				message = static_cast<Network::AddPlayerMessage*>(message);
				break;
			case Network::C_MESSAGE_CHAT:
				message = static_cast<Network::ChatMessage*>(message);
				break;
			case Network::C_MESSAGE_JOIN:
				message = static_cast<Network::JoinMessage*>(message);
				break;
			case Network::C_MESSAGE_LEAVE_GAME:
				message = static_cast<Network::LeaveGameMessage*>(message);
				break;
			case Network::C_MESSAGE_PLACE_PIECE:
				message = static_cast<Network::PlacePieceMessage*>(message);
				break;
			case Network::C_MESSAGE_REFUSE:
				message = static_cast<Network::RefuseMessage*>(message);
				break;
			case Network::C_MESSAGE_REMOVE_PIECE:
				message = static_cast<Network::RemovePieceMessage*>(message);
				break;
			case Network::C_MESSAGE_REMOVE_PLAYER:
				message = static_cast<Network::RemovePlayerMessage*>(message);
				break;
			case Network::C_MESSAGE_SET_MARKER:
				message = static_cast<Network::SetMarkerMessage*>(message);
				break;
			case Network::C_MESSAGE_SET_TEAM:
				message = static_cast<Network::SetTeamMessage*>(message);
				break;
			case Network::C_MESSAGE_START_GAME:
				message = static_cast<Network::StartGameMessage*>(message);
				break;
			case Network::C_MESSAGE_STAY_ALIVE:
				message = static_cast<Network::StayAliveMessage*>(message);
				break;
			case Network::C_MESSAGE_TURN:
				message = static_cast<Network::TurnMessage*>(message);
				break;
			}
		}
	}

	void ServerSession::ClientConnected(int slot)
	{
		mTimeoutCounter[slot] = C_TIMEOUT;
		if (slot >= mPlayers.size())
		{
			mServer->Send(slot, RefuseMessage(RefuseReason::TooManyPlayers));
			mClientsToBeRemoved.push_back(slot);
		}
		else
		{

		}
	}

	void ServerSession::ClientDisconnected(int slot)
	{

	}
}

#include "ServerSession.hpp"
#include "MessageInclude.hpp"
#include <sstream>
#include <algorithm>

using namespace Network;

namespace Logic
{
	const float ServerSession::C_TIMEOUT = 5.0f;

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
		// Update the server
		mServer->Update();

		// Handle all messages
		Network::SlotMessage message;
		while ((message = mServer->PopMessage()).mMessage != NULL)
		{
			// If we've received a message, update timeout (unless they're marked to be removed)
			if (std::find(mClientsToBeRemoved.begin(), mClientsToBeRemoved.end(), message.mSlot) == mClientsToBeRemoved.end())
			{
				mTimeoutCounters[message.mSlot] = C_TIMEOUT;
			}

			// Parse the message content
			switch (message.mMessage->ID())
			{
				case Network::C_MESSAGE_CHAT:
				{
					Network::ChatMessage* m = static_cast<Network::ChatMessage*>(message.mMessage);
				} break;

				case Network::C_MESSAGE_JOIN:
				{
					Network::JoinMessage* m = static_cast<Network::JoinMessage*>(message.mMessage);
				
					HandleJoinMessage(message.mSlot, m->mName);
				} break;
				
				case Network::C_MESSAGE_LEAVE_GAME:
				{
					Network::LeaveGameMessage* m = static_cast<Network::LeaveGameMessage*>(message.mMessage);
				} break;

				case Network::C_MESSAGE_PLACE_PIECE:
				{
					Network::PlacePieceMessage* m = static_cast<Network::PlacePieceMessage*>(message.mMessage);
				} break;

				case Network::C_MESSAGE_SET_MARKER:
				{
					Network::SetMarkerMessage* m = static_cast<Network::SetMarkerMessage*>(message.mMessage);
				} break;

				case Network::C_MESSAGE_SET_TEAM:
				{
					Network::SetTeamMessage* m = static_cast<Network::SetTeamMessage*>(message.mMessage);
				} break;
			}
		}

		// Decrease timeout
		float dt = gameTime.GetTimeSinceLastTick().Seconds;
		for (TimeoutMap::iterator it = mTimeoutCounters.begin(); it != mTimeoutCounters.end(); ++it)
		{
			it->second -= dt;
			if (it->second <= 0.0f)
			{
				mServer->DisconnectClient(it->first);
				
				mTimeoutCounters.erase(it);
				it--;
			}
		}
	}

	void ServerSession::ClientConnected(Network::Slot slot)
	{
		mTimeoutCounters[slot] = C_TIMEOUT;
		if (slot >= mPlayers.size())
		{
			mServer->Send(slot, RefuseMessage(RefuseReason::TooManyPlayers));
			mClientsToBeRemoved.push_back(slot);
		}
	}

	void ServerSession::ClientDisconnected(Network::Slot slot)
	{

	}

	void ServerSession::HandleJoinMessage(Network::Slot clientSlot, const std::string& name)
	{
		bool playerValid = true;
		for (unsigned int i = 0; i < mPlayers.size(); ++i)
		{
			if (mPlayers[i].GetName() == name)
			{
				mClientsToBeRemoved.push_back(clientSlot);
				mServer->Send(clientSlot, RefuseMessage(RefuseReason::InvalidName));
				playerValid = false;
			}
		}

		if (playerValid)
		{
			mServer->Send(clientSlot, AcceptMessage(mPlayers.size(), clientSlot));
			mPlayers[clientSlot].SetName(name);
			mPlayers[clientSlot].SetController(NULL);	// TODO: Set RemoteController

			for (unsigned int i = 0; i < mPlayers.size(); ++i)
			{
				if (i != clientSlot)
				{
					mServer->Send(clientSlot, AddPlayerMessage(i, mPlayers[i].GetTeam(), mPlayers[i].GetMarkerType(), mPlayers[i].GetName()));
				}
			}
		}
	}
}

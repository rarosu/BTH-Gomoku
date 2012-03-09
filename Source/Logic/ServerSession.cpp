#include "ServerSession.hpp"
#include "MessageInclude.hpp"
#include <sstream>
#include <algorithm>

using namespace Network;

namespace Logic
{
	const float ServerSession::C_TIMEOUT = 1.0f;

	ServerSession::ServerSession(Network::Server* server, const std::string& adminName, Ruleset* ruleset) 
		: Session(ruleset->GetPlayerCount())
		, mServer(server)
		, mRuleset(ruleset)
	{
		mServer->SetEventInterface(this);

		mSlotStatus.resize(mRuleset->GetPlayerCount());
		//mTimeoutCounters.resize(mRuleset->GetPlayerCount());

		mPlayers[0] = new Player();
		mPlayers[0]->SetName(adminName);
		
		mSlotStatus[0] = C_STATUS_LOCAL;
		//mTimeoutCounters[0] = -1.0f;
		for (Slot s = 1; s < mPlayers.size(); ++s)
		{
			mSlotStatus[s] = C_STATUS_OPEN;
			//mTimeoutCounters[s] = -1.0f;
		}
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
			// Get the player this message is about
			//PlayerSlot slot = GetPlayerSlot(message.mSlot);

			// If we've received a message, update timeout (unless they're marked to be removed)
			/*
			if (std::find(mClientsToRemove.begin(), mClientsToRemove.end(), message.mSlot) == mClientsToRemove.end())
			{
				mTimeoutCounters[slot] = C_TIMEOUT;
			}
			*/

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

			SafeDelete(message.mMessage);
		}

		// Decrease timeout
		float dt = gameTime.GetTimeSinceLastTick().Seconds;
		for (TimeoutMap::iterator it = mTimeoutCounters.begin(); it != mTimeoutCounters.end(); ++it)
		{
			it->second -= dt;
			if (it->second <= 0.0f)
			{
				mServer->DisconnectClient(mSlotMap[it->first]);

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
		SafeDelete(mPlayers[mSlotMap[slot]]);
	}

	ServerSession::PlayerSlot ServerSession::GetPlayerSlot(ClientSlot slot) const
	{
		for (PlayerSlot i = 0; i < mSlots.size(); ++i)
		{
			if (mSlots[i] == slot)
				return i;
		}

		return -1;
	}

	void ServerSession::HandleJoinMessage(Network::Slot clientSlot, const std::string& name)
	{
		bool playerValid = true;
		RefuseReason::RefuseReason reason = RefuseReason::TooManyPlayers;
		PlayerSlot s = 0;

		// Find an open slot
		for (s = 0; s < mPlayers.size(); ++s)
		{
			if (mSlotMap[s] == C_SLOT_OPEN)
			{
				break;
			}

			if (s == mPlayers.size() - 1)
			{
				playerValid = false;
				reason = RefuseReason::TooManyPlayers;
			}
		}
		
		// If an open slot is found, check the player's name
		if (playerValid)
		{
			for (unsigned int i = 0; i < mPlayers.size(); ++i)
			{
				if (mPlayers[i] != NULL && mPlayers[i]->GetName() == name)
				{
					playerValid = false;
					break;
				}
			}
		}

		// If the player is valid, add them to the open slot
		if (playerValid)
		{
			mServer->Send(clientSlot, AcceptMessage(mPlayers.size(), clientSlot));
			mPlayers[clientSlot] = new Player();
			mPlayers[clientSlot]->SetName(name);
			mSlotMap[s] = clientSlot;

			for (PlayerSlot i = 0; i < mPlayers.size(); ++i)
			{
				if (mPlayers[i] != NULL && i != clientSlot)
				{
					mServer->Send(clientSlot, AddPlayerMessage(i, mPlayers[i]->GetTeam(), mPlayers[i]->GetMarkerType(), mPlayers[i]->GetName()));
				}
			}
		}
		else
		{
			mClientsToBeRemoved.push_back(clientSlot);
			mServer->Send(clientSlot, RefuseMessage(reason));
		}
	}
}

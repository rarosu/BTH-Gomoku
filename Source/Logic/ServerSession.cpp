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

		mPlayers[0] = new Player();
		mPlayers[0]->SetName(adminName);
		
		mPlayerClients[0] = C_STATUS_LOCAL;
		for (Slot s = 1; s < mPlayers.size(); ++s)
		{
			mPlayerClients[s] = C_STATUS_OPEN;
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
		/*
		float dt = gameTime.GetTimeSinceLastTick().Seconds;
		for (TimeoutMap::iterator it = mTimeoutCounters.begin(); it != mTimeoutCounters.end(); ++it)
		{
			it->second -= dt;
			if (it->second <= 0.0f)
			{
				mServer->DisconnectClient(mSlotMap[it->first]);

			}
		}
		*/
	}

	const std::string& ServerSession::GetPlayerName(unsigned int playerSlot) const
	{
		if (mPlayers[playerSlot] == NULL)
			return "";
		return mPlayers[playerSlot]->GetName();
	}

	void ServerSession::ClientConnected(Network::Slot slot)
	{
		//mTimeoutCounters[slot] = C_TIMEOUT;
		if (slot >= mPlayers.size())
		{
			mServer->Send(slot, RefuseMessage(RefuseReason::TooManyPlayers));
			//mClientsToBeRemoved.push_back(slot);
		}
	}

	void ServerSession::ClientDisconnected(Network::Slot slot)
	{
		SafeDelete(mPlayers[GetPlayerSlot(slot)]);
	}

	ServerSession::PlayerSlot ServerSession::GetPlayerSlot(ClientSlot slot) const
	{
		for (SlotMap::const_iterator it = mPlayerClients.begin(); it != mPlayerClients.end(); ++it)
		{
			if (it->second == slot)
				return it->first;
		}

		return C_INVALID_PLAYER;
	}

	void ServerSession::HandleJoinMessage(Network::Slot clientSlot, const std::string& name)
	{
		bool playerValid = true;
		RefuseReason::RefuseReason reason;

		// Find an open slot
		PlayerSlot openPlayerSlot = GetPlayerSlot(C_STATUS_OPEN);
		if (openPlayerSlot == C_INVALID_PLAYER)
		{
			// There are no open slots
			playerValid = false;
			reason = RefuseReason::TooManyPlayers;
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
			mServer->Send(clientSlot, AcceptMessage(mPlayers.size(), openPlayerSlot));
			mPlayers[openPlayerSlot] = new Player();
			mPlayers[openPlayerSlot]->SetName(name);
			mPlayerClients[openPlayerSlot] = clientSlot;

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
			mClientsToRemove.push_back(clientSlot);
			mServer->Send(clientSlot, RefuseMessage(reason));
		}
	}
}

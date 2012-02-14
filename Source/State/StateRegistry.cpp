#include "StateRegistry.hpp"

namespace State
{
	bool StateRegistry::RegisterState(StateID id, ApplicationState* state)
	{
		if (mStates.find(id) != mStates.end())
			return false;

		mStates[id] = state;

		return true;
	}

	void StateRegistry::UnregisterState(StateID id)
	{
		mStates.erase(id);
	}

	ApplicationState* StateRegistry::GetState(StateID id) const
	{
		StateMap::const_iterator it = mStates.find(id);

		if (it != mStates.end())
			return it->second;
		
		return NULL;
	}
}


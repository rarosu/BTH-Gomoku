#ifndef STATE_REGISTRY_HPP
#define STATE_REGISTRY_HPP

#include <map>

namespace State
{
	typedef int StateID;

	/**
		Forward-declare state
	*/
	class ApplicationState;

	/**
		Associate an ApplicationState object with a StateID.
	*/
	class StateRegistry
	{
	public:
		/**
			Register a state with the specified ID. If a state with
			that ID already exist, return false. Otherwise, return true.
		*/
		bool RegisterState(StateID id, ApplicationState* state);

		/**
			Unregister a state with the specified ID, if it is registered.
			Otherwise, do nothing.
		*/
		void UnregisterState(StateID id);

		/**
			Return the state with the given ID, or NULL if none exist.
		*/
		ApplicationState* GetState(StateID id) const;
	private:
		typedef std::map<StateID, ApplicationState*> StateMap;

		StateMap mStates;
	};
}

#endif
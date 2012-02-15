#ifndef APPLICATION_STATE_HPP
#define APPLICATION_STATE_HPP

#include "StateStack.hpp"
#include "StateRegistry.hpp"
#include "GameTime.hpp"

/**
	Forward-declare
*/
class Game;
class InputManager;

namespace State
{
	/**
		Constants for all states in Gomoku
	*/
	const StateID C_STATE_MENU = 1;
	const StateID C_STATE_LOCAL_LOBBY = 2;
	const StateID C_STATE_IN_GAME = 3;

	/**
		Abstract class for an application state

		Every state the application can be in should
		extend this class.
	*/
	class ApplicationState
	{
	public:
		/**
			Game has to be a friend of this class, to be able to notify
			the stack about updating and drawing.
		*/
		friend class Game;

		/**
			Destructor - remove this state from the registry
		*/
		~ApplicationState() throw();

		/**
			These are called when the state is pushed or popped from
			the stack
		*/
		virtual void OnStatePushed();
		virtual void OnStatePopped();

		/**
			Handle update and draw events in extended classes.
		*/
		virtual void Update(const InputManager& input, const GameTime& gameTime) = 0;
		virtual void Draw() = 0; 

		/**
			These can be used to override default behaviour
		*/
		virtual bool UpdateStateBeneath();
		virtual bool DrawStateBeneath();
	protected:
		/**
			Constructor - add this state to the registry
		*/
		explicit ApplicationState(StateID id);

		/**
			These methods can be used to push/pop/change states
			by using their IDs.
		*/
		static void PushState(StateID id);
		static void PopState();
		static void ChangeState(StateID id);
		static void QuitApplication();

	private:
		/**
			One instance of a registry and a stack
		*/
		static StateRegistry sRegistry;
		static StateStack sStack;

		/**
			The ID of this state
		*/
		StateID mStateID;

		/**
			This class cannot be copied - since only one state
			with a certain ID should be able to exist.
		*/
		ApplicationState(const ApplicationState& copy) {}
		ApplicationState& operator=(const ApplicationState& copy) {}
	};
}

#endif
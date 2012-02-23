#ifndef APPLICATION_STATE_HPP
#define APPLICATION_STATE_HPP

#include "GameTime.hpp"
#include "StateRegistry.hpp"
#include "StateStack.hpp"
#include "Viewport.hpp"
#include "InputManager.hpp"

/**
	Forward-declare
*/
class Game;

namespace State
{
	/**
		Constants for all states in Gomoku
	*/
	const StateID C_STATE_MENU			= 1;
	const StateID C_STATE_LOCAL_LOBBY	= 2;
	const StateID C_STATE_NETWORK_LOBBY = 3;
	const StateID C_STATE_IN_GAME		= 4;

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
		virtual void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime) = 0;
		virtual void Draw() = 0;

		/**
			This will be called when the application has been resized, and the Viewport has
			been updated.
		*/
		virtual void OnResize();

		/**
			These can be used to override default behaviour
		*/
		virtual bool UpdateStateBeneath();
		virtual bool DrawStateBeneath();
	protected:
		static const Viewport*				sViewport;
		static InputSubscription*			sInputManager;

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
#ifndef APPLICATION_STATE_HPP
#define APPLICATION_STATE_HPP

#include "GameTime.hpp"
#include "ComponentGroup.hpp"
#include "StateRegistry.hpp"
#include "Viewport.hpp"
#include "InputManager.hpp"

/**
	Forward-declare
*/
class Game;

namespace State
{
	/**
		Forward-declare
	*/
	class ApplicationState;

	/**
		Constants for all states in Gomoku
	*/
	const StateID C_STATE_MENU			= 1;
	const StateID C_STATE_SERVER_LOBBY	= 2;
	const StateID C_STATE_CLIENT_LOBBY	= 3;
	const StateID C_STATE_SERVER_GAME	= 4;
	const StateID C_STATE_CLIENT_GAME	= 5;
	const StateID C_STATE_CREATE_GAME	= 6;
	const StateID C_STATE_JOIN_GAME		= 7;


	/**
		This should be implemented by the Game class,
		to handle changing of states.
	*/
	class StateManager
	{
	public:
		virtual void ChangeState(ApplicationState* state) = 0;
	};

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
	protected:
		static const Viewport*				sViewport;
		static InputSubscription*			sInputManager;
		static Components::ComponentGroup*	sRootComponentGroup;

		/**
			Constructor - add this state to the registry
		*/
		explicit ApplicationState(StateID id);

		/**
			These methods can be used to change states
			by using their IDs.
		*/
		static void ChangeState(StateID id);
		static void QuitApplication();
	private:
		/**
			One instance of a registry and a stack
		*/
		static StateRegistry sRegistry;
		static StateManager* sManager;

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
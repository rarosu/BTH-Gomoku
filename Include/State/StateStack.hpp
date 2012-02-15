#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP

#include <vector>
#include "GameTime.hpp"

/**
	Forward-declare
*/
class InputManager;


namespace State
{
	/**
		Forward-declare
	*/
	class ApplicationState;
	

	/**
		A stack of application states - the top state
		will be updated.
		
		Several states can be stacked on top of eachother, 
		to make going into and return to states easy.
	*/
	class StateStack
	{
	public:
		/**
			Push a state on top of the stack - do not
			alter the stack in any other way.
		*/
		void PushState(ApplicationState* state);

		/**
			Pop the topmost state - if there are no states
			on the stack, do nothing.
		*/
		void PopState();

		/**
			Clear the entire stack and push the
			given state as the only state.
		*/
		void ChangeState(ApplicationState* state);


		/**
			Get the topmost state
		*/
		ApplicationState* Top();
		const ApplicationState* Top() const;

		
		/**
			Update the topmost state
		*/
		void UpdateState(const InputManager& input, const GameTime& gameTime);

		/**
			Draw the topmost state
		*/
		void DrawState();


		/**
			Update the stack - perform the queued actions
		*/
		void UpdateStack();

		/**
			Get the next state that will be pushed when
			the stack is updated.

			If this is NULL, it means all states were popped off, and
			that the application should exit.
		*/
		ApplicationState* GetNextState() const;
	private:
		typedef std::vector<ApplicationState*> StateVector;

		/**
			The actual stack of states
		*/
		StateVector mStack;

		/**
			The stack, as it will look next frame. Stores
			changes made by Push/Pop/ChangeState.
		*/
		StateVector mNextStack;
	};
}

#endif
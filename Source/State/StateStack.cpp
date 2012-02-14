#include "StateStack.hpp"
#include "ApplicationState.hpp"
#include "Globals.hpp"
#include <algorithm>
#include <cmath>

namespace State
{
	void StateStack::PushState(ApplicationState* state)
	{
		mNextStack.push_back(state);
	}

	void StateStack::PopState()
	{
		if (!mNextStack.empty())
		{
			mNextStack.pop_back();
		}
			
	}

	void StateStack::ChangeState(ApplicationState* state)
	{
		mNextStack.erase(mNextStack.begin(), mNextStack.end());

		PushState(state);
	}


	ApplicationState* StateStack::Top()
	{
		return mStack.back();
	}

	const ApplicationState* StateStack::Top() const
	{
		return mStack.back();
	}

		
	void StateStack::UpdateState(const InputManager& input, const GameTime& gameTime)
	{
		// TODO: Need to update states below top state, if it allows for it
		mStack.back()->Update(input, gameTime);
	}

	void StateStack::DrawState()
	{
		// TODO: Need to draw states below top state, if it allows for it
		mStack.back()->Draw();
	}

	
	template <typename T>
	std::vector<T> OrderedIntersect(const std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		std::vector<T> result;
		for (int i = 0; i < std::min<int>(lhs.size(), rhs.size()); ++i)
		{
			if (lhs[i] == rhs[i])
			{
				result.push_back(lhs[i]);
			}
		}

		return result;
	}

	template <typename T>
	std::vector<T> OrderedDifference(const std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		std::vector<T> result;
		for (int i = 0; i < lhs.size(); ++i)
		{
			if (i >= rhs.size())
			{
				result.push_back(lhs[i]);
			}
			else
			{
				if (lhs[i] != rhs[i])
				{
					result.push_back(lhs[i]);
				}
			}
		}

		return result;
	}

	void StateStack::UpdateStack()
	{
		// Compare the changes made to NextStack, and call OnPop/OnPush appropriately
		StateVector movedElements(mStack.size());
		StateVector removedElements(mStack.size());
		StateVector addedElements(mStack.size());

		StateVector intersection(mStack.size());
		StateVector orderedIntersection(mStack.size());

		StateVector sortedStack = mStack;
		StateVector sortedNextStack = mNextStack;

		/**
			Let A be the current state stack, and B be the next state stack.

			We want to compare these ordered sets and find out which elements
			were removed, added or moved.

			(A intersect B) \ (A ordered intersect B)	.. are the elements that were moved
			A \ B										.. are the elements that were removed
			B \ A										.. are the elements that were added

			Ordered intersect compares the elements at the same position, and if they coincide,
			it is added to the intersection.
		*/

		std::sort(sortedStack.begin(), sortedStack.end());
		std::sort(sortedNextStack.begin(), sortedNextStack.end());

		intersection = OrderedIntersect(mStack, mNextStack);
		orderedIntersection = OrderedIntersect(sortedStack, sortedNextStack);

		movedElements = OrderedDifference(intersection, orderedIntersection);
		removedElements = OrderedDifference(sortedStack, sortedNextStack);
		addedElements = OrderedDifference(sortedNextStack, sortedStack);


		// Update the states by calling OnPop/OnPush
		for (StateVector::iterator it = movedElements.begin(); it != movedElements.end(); ++it)
		{
			(*it)->OnStatePopped();
			(*it)->OnStatePushed();
		}

		for (StateVector::iterator it = removedElements.begin(); it != removedElements.end(); ++it)
		{
			(*it)->OnStatePopped();
		}
		
		for (StateVector::iterator it = addedElements.begin(); it != addedElements.end(); ++it)
		{
			(*it)->OnStatePushed();
		}

		// Switch next stack to the current stack
		mStack = mNextStack;
	}

	ApplicationState* StateStack::GetNextState() const
	{
		return mNextStack.back();
	}
}


#include "ApplicationState.hpp"
#include <stdexcept>

namespace State
{
	StateRegistry				ApplicationState::sRegistry;
	StateStack					ApplicationState::sStack;
	const Viewport*				ApplicationState::sViewport		= NULL;
	InputSubscription*			ApplicationState::sInputManager	= NULL;

	ApplicationState::ApplicationState(StateID id)
	{
		if (!sRegistry.RegisterState(id, this))
			throw std::logic_error("Two application states can not have the same ID");
		
		mStateID = id;
	}

	ApplicationState::~ApplicationState() throw()
	{
		sRegistry.UnregisterState(mStateID);
	}



	void ApplicationState::OnStatePushed() {}
	void ApplicationState::OnStatePopped() {}

	bool ApplicationState::UpdateStateBeneath()
	{
		return false;
	}

	bool ApplicationState::DrawStateBeneath()
	{
		return false;
	}



	void ApplicationState::PushState(StateID id)
	{
		sStack.PushState(sRegistry.GetState(id));
	}

	void ApplicationState::PopState()
	{
		sStack.PopState();
	}

	void ApplicationState::ChangeState(StateID id)
	{
		sStack.ChangeState(sRegistry.GetState(id));
	}

	void ApplicationState::QuitApplication()
	{
		sStack.ChangeState(NULL);
	}

}
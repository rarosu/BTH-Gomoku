#include "ApplicationState.hpp"
#include <stdexcept>

namespace State
{
	StateRegistry				ApplicationState::sRegistry;
	StateManager*				ApplicationState::sManager				= NULL;
	const Viewport*				ApplicationState::sViewport				= NULL;
	InputSubscription*			ApplicationState::sInputManager			= NULL;
	Components::ComponentGroup*	ApplicationState::sRootComponentGroup	= NULL;

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

	void ApplicationState::OnResize() {}

	void ApplicationState::ChangeState(StateID id)
	{
		sManager->ChangeState(sRegistry.GetState(id));
	}

	void ApplicationState::QuitApplication()
	{
		sManager->ChangeState(NULL);
	}

}
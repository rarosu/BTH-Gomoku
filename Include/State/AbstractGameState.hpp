#ifndef ABSTRACT_GAME_STATE_HPP
#define ABSTRACT_GAME_STATE_HPP

#include "ApplicationState.hpp"
#include "Scene.hpp"
#include "ComponentGroup.hpp"
#include "Console.hpp"
#include "Session.hpp"

namespace State
{
	class AbstractGameState : public ApplicationState
	{
	public:
		AbstractGameState(StateID id, ID3D10Device* device);
		~AbstractGameState() throw();

		void OnStatePushed();
		void OnStatePopped();

		void OnResize();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();
	protected:
		void SetSession(Logic::Session* session);
	private:
		ID3D10Device* mDevice;
		Logic::Session* mSession;

		Components::ComponentGroup*	mComponents;
		Scene* mScene;
		Components::Console* mChat;

		void CreateComponents();
		float GetAspectRatio();
	};
}

#endif
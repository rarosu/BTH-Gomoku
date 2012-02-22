#ifndef MAIN_STATE_HPP
#define MAIN_STATE_HPP

#include "ApplicationState.hpp"
#include "Grid.hpp"
#include "Scene.hpp"
#include "Marker.hpp"
#include "Menu.hpp"

namespace State
{
	class InGameState : public ApplicationState
	{
	public:
		InGameState(StateID id, ID3D10Device* device, const Frustrum& viewFrustrum, InputSubscription* inputSubscription);
		~InGameState() throw();

		void OnStatePushed();					// Initialize
		void OnStatePopped();					// Destroy

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void SetViewFrustrum(const Frustrum& viewFrustrum);

	private:
		ID3D10Device*		mDevice;
		InputSubscription*	mInputSubscription;

		Logic::Grid*		mGrid;

		Scene*				mScene;
		Camera*				mCamera;
		Marker*				mMarker;		// Debug
		Frustrum			mViewFrustrum;

		Components::Menu*	mDragonAgeMenu;
		bool				mShowMenu;
	};
}

#endif
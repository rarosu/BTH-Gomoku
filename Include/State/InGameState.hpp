#ifndef MAIN_STATE_HPP
#define MAIN_STATE_HPP

#include "ApplicationState.hpp"
#include "Grid.hpp"
#include "Scene.hpp"
#include "Marker.hpp"
#include "ComponentGroup.hpp"
#include "Menu.hpp"

namespace State
{
	class InGameState : public ApplicationState
	{
	public:
		InGameState(StateID id, ID3D10Device* device);
		~InGameState() throw();

		void OnStatePushed();					// Initialize
		void OnStatePopped();					// Destroy

		void OnResize();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();
	private:
		ID3D10Device*					mDevice;

		Logic::Grid*					mGrid;

		Scene*							mScene;
		Camera*							mCamera;
		Marker*							mMarkerBlue;		// Debug
		Marker*							mMarkerGreen;
		Frustrum						mViewFrustrum;

		Components::ComponentGroup*		mComponents;
		Components::Menu*				mDragonAgeMenu;
		bool							mShowMenu;
	};
}

#endif
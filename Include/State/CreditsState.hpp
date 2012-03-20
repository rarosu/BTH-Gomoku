#ifndef CREDITS_STATE_HPP
#define CREDITS_STATE_HPP

#include "WinInclude.hpp"
#include "ApplicationState.hpp"
#include "Sprite.hpp"
#include "GameFont.hpp"
#include "TextButton.hpp"

namespace State
{
	class CreditsState : public ApplicationState
	{
	public:
		CreditsState(StateID id, ID3D10Device* device);
		~CreditsState() throw();
		
		void OnStatePushed();
		void OnStatePopped();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();
	private:
		struct bgVertex
		{
			D3DXVECTOR2		position;
			D3DXVECTOR2		uv;
		};

		ID3D10Device*				mDevice;
		GameFont*					mDefaultFont;
		Sprite*						mBackground;

		Components::ComponentGroup*	mComponents;
		Components::TextButton*		mBtnBack;

		void CreateComponents();
	};
}


#endif
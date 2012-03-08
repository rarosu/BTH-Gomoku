#ifndef JOIN_GAME_STATE_HPP
#define JOIN_GAME_STATE_HPP

#include "WinInclude.hpp"
#include "ApplicationState.hpp"
#include "Sprite.hpp"
#include "InputField.hpp"
#include "TextButton.hpp"

namespace State
{
	class JoinGameState : public ApplicationState
	{
	public:
		JoinGameState(StateID id, ID3D10Device* device);
		~JoinGameState() throw();
		
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

		Components::InputField*		mNameField;
		Components::InputField*		mIPAddressField;
		Components::InputField*		mPortField;
		Components::TextButton*		mJoinButton;
		Components::TextButton*		mCancelButton;



		void CreateComponents();
	};
}


#endif
#ifndef CREATE_A_GAME_STATE_HPP
#define CREATE_A_GAME_STATE_HPP

#include "ApplicationState.hpp"
#include "TextButton.hpp"
#include "GameFont.hpp"
#include "Label.hpp"
#include "InputField.hpp"
#include "GameFont.hpp"
#include "Sprite.hpp"

namespace State
{
	namespace CAGButton
	{
		enum Button
		{
			Create,
			Cancel,
			Count
		};
	}

	class CreateAGameState : public ApplicationState
	{
	public:
		CreateAGameState(StateID id, ID3D10Device* device);

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();

	private:
		struct bgVertex
		{
			D3DXVECTOR2		position;
			D3DXVECTOR2		uv;
		};

		ID3D10Device*							mDevice;
		Components::ComponentGroup*				mComponents;
		std::vector<Components::TextButton*>	mButtons;
		Sprite*									mBackground;
		GameFont*								mDefaultFont;
		Components::Label*						mLblName;
		Components::InputField*					mIFName;

		void CreateComponents();
	};
}
#endif
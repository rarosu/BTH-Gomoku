#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "WinInclude.hpp"
#include <vector>

#include "ApplicationState.hpp"
#include "ComponentGroup.hpp"
#include "TextButton.hpp"
#include "Menu.hpp"
#include "ClickMenu.hpp"
#include "Sprite.hpp"

namespace State
{
	namespace MenuButton
	{
		enum Button
		{
			StartGame,
			WatchReplay,
			Credits,
			Exit,
			Count
		};
	}

	class MenuState : public ApplicationState
	{
	public:
		MenuState(StateID id, ID3D10Device* device);
		~MenuState() throw();
		
		
		// Inherited from Application State
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

		static const std::string C_MENU_CAPTIONS[];

		ID3D10Device*							mDevice;
		std::vector<Components::MenuItem*>		mButtons;
		Components::ClickMenu*					mMenuButtons;
		Components::ComponentGroup*				mComponents;
		Sprite*									mBackground;

		void CreateComponents();
	};
}

#endif
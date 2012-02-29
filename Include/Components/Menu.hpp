#ifndef MENU_HPP
#define MENU_HPP

#include <vector>

#include "Component.hpp"
#include "Button.hpp"
#include "Label.hpp"

namespace Components
{
	class Menu : public Component
	{
	public:
		Menu(ID3D10Device* device, ComponentGroup* ownerGroup, RECT position);

		void SetPosition(RECT newPosition);

		// Methods inherited from Component
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		void LostFocus();
		void GotFocus();

	private:
		ID3D10Device*						mDevice;

		std::vector<Components::Button*>	mButtons;
		std::vector<Button::Graphics>		mGraphics;
		std::vector<RECT>					mPositions;

		void UpdateButtonPositions();
		void CreateButtons();
	};
}
#endif
#ifndef STANDARDBUTTON_HPP
#define STANDARDBUTTON_HPP

#include <string>

#include "Buffer.hpp"
#include "Button.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"
#include "InputManager.hpp"

namespace Components
{
	class StandardButton : public Button
	{
	public:
		StandardButton(InputSubscription* manager);
	
		void Initialize(ID3D10Device* device, RECT position, std::string caption);
		//void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();

	private:
		GameFont*					mFont;
		std::string					mCaption;
		D3DXCOLOR					mTextColor;
	};
}
#endif
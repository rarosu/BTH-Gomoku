#ifndef TEXTBUTTON_HPP
#define TEXTBUTTON_HPP

#include <string>

#include "Button.hpp"
#include "GameFont.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"

namespace Components
{
	class TextButton : public Button
	{
	public:
		TextButton(ComponentGroup* ownerGroup);
	
		virtual void Initialize(ID3D10Device* device, RECT position, std::string caption);
		//void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		
		// DEBUG
		virtual std::string GetName();

	private:
		GameFont*					mFont;
		std::string					mCaption;
		D3DXCOLOR					mTextColor;
	};
}
#endif
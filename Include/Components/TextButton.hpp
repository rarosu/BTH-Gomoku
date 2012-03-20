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
		TextButton(ComponentGroup* ownerGroup, RECT position);
	
		virtual void Initialize(ID3D10Device* device, std::string caption);
		void Draw();
		void SetCaption(std::string caption);
		
		// DEBUG
		virtual std::string GetName();

	protected:
		// Methods inherited from Component
		virtual void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState) {}

	private:
		GameFont*					mFont;
		std::string					mCaption;
		D3DXCOLOR					mTextColor;
	};
}
#endif
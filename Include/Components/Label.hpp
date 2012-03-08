#ifndef LABEL_HPP
#define LABEL_HPP

#include <string>

#include "WinInclude.hpp"
#include "Component.hpp"
#include "GameFont.hpp"

namespace Components
{
	class Label : public Component
	{
	public:
		Label(ID3D10Device* device,
			  ComponentGroup* ownerGroup,
			  std::string caption, 
			  RECT position, 
			  GameFont::AlignHorizontal alignHorizontal = GameFont::Center, 
			  GameFont::AlignVertical alignVertical = GameFont::Middle);
		
		std::string GetCaption();
		void SetCaption(std::string newCaption);

		// Methods inherited from Component
		virtual void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		virtual void Draw();
		void LostFocus();
		void GotFocus();
		// DEBUG
		virtual std::string GetName();

	private:
		GameFont*					mFont;
		RECT						mShadowPosition;
		std::string					mCaption;
		D3DXCOLOR					mTextColor;
		D3DXCOLOR					mShadowColor;
		GameFont::AlignHorizontal	mAlignHor;
		GameFont::AlignVertical		mAlignVer;

		// Methods inherited by Component
		void MouseEntered() {}
		void MouseExited() {}
		void MousePressed(int buttonIndex) {}
		void MouseReleased(int buttonIndex) {}
	};
}
#endif
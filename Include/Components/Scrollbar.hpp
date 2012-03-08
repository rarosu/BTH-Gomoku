#ifndef SCROLLBAR_HPP
#define SCROLLBAR_HPP

#include "Component.hpp"
#include "Button.hpp"
#include "Sprite.hpp"

namespace Components
{
	class Scrollable
	{
	public:
		virtual void Scroll(bool isUp) = 0;
	};

	class Scrollbar : public Component
	{
	public:
		Scrollbar(ComponentGroup* ownerGroup, Scrollable* scrollable, RECT position);
		~Scrollbar() throw();

		void Initialize(ID3D10Device* device);

		// Methods inherited from Component
		void Draw();
		virtual void LostFocus();
		virtual void GotFocus();
		// DEBUG
		virtual std::string GetName();

	protected:
		// Methods inherited from Component
		void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void MouseEntered();
		void MouseExited();
		void MousePressed(int buttonIndex);
		void MouseReleased(int buttonIndex);

	private:
		ID3D10Device*				mDevice;
		Sprite*						mBackground;
		D3DXCOLOR					mBGColor;
		float						mBGDrawOffset;

		Button*						mBtnUp;
		Button*						mBtnDown;
		Scrollable*					mScrollable;
	};
}
#endif
#ifndef SCROLLBAR_HPP
#define SCROLLBAR_HPP

#include "Button.hpp"
#include "Clickable.hpp"
#include "Sprite.hpp"

namespace Components
{
	class Scrollable
	{
	public:
		virtual void Scroll(bool isUp) = 0;
	};

	class Scrollbar : public Clickable
	{
	public:
		Scrollbar(ComponentGroup* ownerGroup, Scrollable* scrollable);
		~Scrollbar() throw();

		void Initialize(ID3D10Device* device, RECT position);

		// Methods inherited from Component
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		virtual void LostFocus();
		virtual void GotFocus();
		// DEBUG
		virtual std::string GetName();

	protected:
		// Methods inherited from Clickable
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
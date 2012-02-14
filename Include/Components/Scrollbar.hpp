#ifndef SCROLLBAR_HPP
#define SCROLLBAR_HPP

#include "Button.hpp"
#include "Clickable.hpp"

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
		Scrollbar(InputSubscription* manager, Scrollable* scrollable);
		void Initialize(ID3D10Device* device, RECT position);
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		virtual void LostFocus();
		virtual void GotFocus();

	protected:
		void MouseEntered();
		void MouseExited();
		void MousePressed(int buttonIndex);
		void MouseReleased(int buttonIndex);

	private:
		ID3D10Device*				mDevice;
		Buffer*						mBuffer;
		Effect*						mEffect;

		Button*						mBtnUp;
		Button*						mBtnDown;
		Scrollable*					mScrollable;

		void CreateBuffer();
		void CreateEffect();
	};
}
#endif
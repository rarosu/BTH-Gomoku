#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "WinInclude.hpp"
#include "Component.hpp"
#include "Clickable.hpp"
#include "Sprite.hpp"

namespace Components
{
	class Button : public Clickable
	{
	public:
		struct Graphics
		{
			D3DXCOLOR					idleColor;
			D3DXCOLOR					activeColor;
			D3DXCOLOR					hoverColor;
			Sprite*						textureUp;
			Sprite*						textureDown;
			Sprite*						textureHover;

			Graphics();
		};

		Button(ComponentGroup* ownerGroup);
	
		virtual void Initialize(ID3D10Device* device, RECT position, Graphics buttonColors);

		// Methods inherited from Component
		virtual void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		virtual void Draw();
		virtual void LostFocus();
		virtual void GotFocus();
	
	protected:
		ID3D10Device*				mDevice;

	private:
		struct ButtonVertex
		{
			D3DXVECTOR2				position;
			D3DXVECTOR2				uv;
		};

		Sprite*						mTexture;
		D3DXCOLOR					mTintColor;
		Graphics					mGraphics;

		// Inherited from Clickable
		void MouseEntered();
		void MouseExited();
		void MousePressed(int buttonIndex);
		void MouseReleased(int buttonIndex);

		// DEBUG
		virtual std::string GetName();
	};
}
#endif
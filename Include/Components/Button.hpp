#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "WinInclude.hpp"
#include "Component.hpp"
#include "Sprite.hpp"

namespace Components
{
	class Button : public Component
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

		Button(ComponentGroup* ownerGroup, RECT position);
	
		virtual void Initialize(ID3D10Device* device, Graphics buttonColors);

		// Methods inherited from Component
		virtual void Draw();
		virtual void LostFocus();
		virtual void GotFocus();
		virtual void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
	
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

		// Inherited from Component
		void MouseEntered();
		void MouseExited();
		void MousePressed(int buttonIndex);
		void MouseReleased(int buttonIndex);

		// DEBUG
		virtual std::string GetName();
	};
}
#endif
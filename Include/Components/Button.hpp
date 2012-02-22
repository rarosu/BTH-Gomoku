#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <D3DX10.h>
#include "Buffer.hpp"
#include "Effect.hpp"
#include "Component.hpp"
#include "Clickable.hpp"

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
			ID3D10ShaderResourceView*	textureUp;
			ID3D10ShaderResourceView*	textureDown;

			Graphics();
		};

		Button(InputSubscription* manager);
	
		virtual void Initialize(ID3D10Device* device, RECT position, Graphics buttonColors);
		virtual void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		virtual void Draw();

		virtual void LostFocus();
		virtual void GotFocus();
	
	private:
		struct ButtonVertex
		{
			D3DXVECTOR2				position;
			D3DXVECTOR2				uv;
		};

		ID3D10Device*				mDevice;
		VertexBuffer*				mBuffer;
		Effect*						mEffect;

		Graphics					mGraphics;

		void CreateBuffer();
		void CreateEffect();

		// Inherited from Clickable
		void MouseEntered();
		void MouseExited();
		void MousePressed(int buttonIndex);
		void MouseReleased(int buttonIndex);
	};
}
#endif
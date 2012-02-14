#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>

#include "Buffer.hpp"
#include "Clickable.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"
#include "InputManager.hpp"

class StandardButton : public Clickable
{
public:
	StandardButton(InputSubscription* manager);
	
	void Initialize(ID3D10Device* device, RECT position, std::string caption);
	void Update(const InputState& currInputState, const InputState& prevInputState);
	void Draw();

private:
	struct ButtonVertex
	{
		D3DXVECTOR2				position;
		D3DXVECTOR2				uv;
	};

	ID3D10Device*				mDevice;
	Buffer*						mBuffer;
	Effect*						mEffect;

	GameFont*					mFont;
	std::string					mCaption;
	D3DXCOLOR					mTextColor;
	D3DXCOLOR					mIdleColor;
	D3DXCOLOR					mHoverColor;
	D3DXCOLOR					mActiveColor;


	void CreateBuffer();
	void CreateEffect();
	void MouseEntered();
	void MouseExited();
	void MousePressed(int buttonIndex);
	void MouseReleased(int buttonIndex);
};
#endif
#ifndef INPUTFIELD_HPP
#define INPUTFIELD_HPP

#include <sstream>
#include <string>
#include <D3DX10.h>

#include "Buffer.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"
#include "InputManager.hpp"

class InputReceiver
{
public:
	virtual void RecieveInput(std::string input) = 0;
};

class InputField : public KeyListener
{
public:
	InputField(ID3D10Device* device, 
			   InputSubscription* inputManager, 
			   InputReceiver* receiver, 
			   RECT position, 
			   GameFont* font);
	~InputField() throw();

	//void Update();
	void Draw();

	// Methods inherited from KeyListener
	void KeyPressed(int code, const InputState& currentState);
	void KeyReleased(int code, const InputState& currentState);
	void CharEntered(unsigned char symbol, const InputState& currentState);

private:
	ID3D10Device*				mDevice;
	InputSubscription*			mInputManager;
	Buffer*						mBuffer;
	Effect*						mEffect;

	RECT						mPosition;
	GameFont*					mFont;
	std::stringstream			mStream;
	InputReceiver*				mReceiver;

	static const int			C_NUM_VERTICES;

	void CreateBuffer();
	void CreateEffect();
};
#endif
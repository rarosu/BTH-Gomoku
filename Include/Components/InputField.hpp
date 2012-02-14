#ifndef INPUTFIELD_HPP
#define INPUTFIELD_HPP

#include <sstream>
#include <string>
#include <D3DX10.h>

#include "Buffer.hpp"
#include "Component.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"
#include "GameTime.hpp"
#include "InputManager.hpp"

namespace Components
{
	class InputReciever
	{
	public:
		virtual void RecieveInput(std::string input) = 0;
	};

	class InputField : public Component, public KeyListener
	{
	public:
		InputField(ID3D10Device* device, InputManager* manager, InputReciever* reciever, 
			RECT position, GameFont* font);
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();

		void LostFocus();
		void GotFocus();

		// Methods inherited from KeyListener
		void KeyPressed(int code);
		void KeyReleased(int code);
		void CharEntered(unsigned char symbol);

	private:
		ID3D10Device*				mDevice;
		Buffer*						mBuffer;
		Effect*						mEffect;

		//RECT						mPosition;
		GameFont*					mFont;
		std::stringstream			mStream;
		InputReciever*				mReciever;

		static const int			C_NUM_VERTICES;

		void CreateBuffer();
		void CreateEffect();
	};
}
#endif
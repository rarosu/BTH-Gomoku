#ifndef INPUTFIELD_HPP
#define INPUTFIELD_HPP

#include <sstream>
#include <string>
#include <D3DX10.h>

#include "Buffer.hpp"
#include "Effect.hpp"
#include "Component.hpp"
#include "GameFont.hpp"
#include "GameTime.hpp"
#include "InputManager.hpp"


namespace Components
{
	class InputReceiver
	{
	public:
		virtual void RecieveInput(std::string input) = 0;
	};

	class InputField : public Component, public KeyListener
	{
	public:
		InputField(ID3D10Device* device, InputSubscription* manager, InputReceiver* receiver, 
		RECT position, GameFont* font);
		~InputField() throw();

		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();

		void LostFocus();
		void GotFocus();

		// Methods inherited from KeyListener
		void KeyPressed(int code, const InputState& currentState);
		void KeyReleased(int code, const InputState& currentState);
		void CharEntered(unsigned char symbol, const InputState& currentState);

	private:
		ID3D10Device*				mDevice;
		VertexBuffer*				mBuffer;
		Effect*						mEffect;

		GameFont*					mFont;
		//std::stringstream			mStream;
		std::stringstream			mFirstString;
		std::stringstream			mLastString;
		InputReceiver*				mReceiver;
		InputSubscription*				mManager;
		bool						mShowMarker;
		float						mMSSinceBlink;
		
		static const float			C_MARKER_SPEED;
		static const int			C_NUM_VERTICES;

		void CreateBuffer();
		void CreateEffect();
	};
}
#endif
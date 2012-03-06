#ifndef INPUTFIELD_HPP
#define INPUTFIELD_HPP

#include "WinInclude.hpp"
#include <sstream>
#include <string>

#include "Buffer.hpp"
#include "Effect.hpp"
#include "Clickable.hpp"
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

	class InputField : public Clickable
	{
	public:
		InputField(ID3D10Device* device, ComponentGroup* ownerGroup, InputReceiver* receiver, 
		RECT position, GameFont* font);
		~InputField() throw();

		std::string GetText() const;
		bool Empty() const;
		void SetText(std::string newText);

		// Methods inherited from Component
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		void LostFocus();
		void GotFocus();
		
		// DEBUG
		virtual std::string GetName();

		// Methods inherited from KeyListener
		void KeyPressed(int code, const InputState& currentState);
		void KeyReleased(int code, const InputState& currentState);
		void CharEntered(unsigned char symbol, const InputState& currentState);

		// Methods inherited from Clickable
		void MouseEntered();
		void MouseExited();
		void MousePressed(int buttonIndex);
		void MouseReleased(int buttonIndex);

	private:
		ID3D10Device*				mDevice;
		VertexBuffer*				mBuffer;
		Effect*						mEffect;

		GameFont*					mFont;
		//std::stringstream			mStream;
		std::stringstream			mFirstString;
		std::stringstream			mLastString;
		InputReceiver*				mReceiver;

		bool						mHasFocus;
		bool						mShowMarker;
		float						mMSSinceBlink;
		
		static const float			C_MARKER_SPEED;
		static const int			C_NUM_VERTICES;

		void CreateBuffer();
		void CreateEffect();
	};
}
#endif
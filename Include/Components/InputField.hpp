#ifndef INPUTFIELD_HPP
#define INPUTFIELD_HPP

#include "WinInclude.hpp"
#include <sstream>
#include <string>

#include "Component.hpp"
#include "GameFont.hpp"
#include "GameTime.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"

namespace Components
{
	class InputReceiver
	{
	public:
		virtual void ReceiveInput(std::string input) = 0;
	};

	class InputField : public Component
	{
	public:
		InputField(ID3D10Device* device, ComponentGroup* ownerGroup, InputReceiver* receiver, 
		RECT position, GameFont* font, int maxChars = -1);
		~InputField() throw();

		bool IsEmpty() const;
		std::string GetText() const;
		void SetText(std::string newText);
		void SetMaxCharacters(int maxChars);

		// Methods inherited from Component
		void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
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
		void MousePressed(int buttonIndex, const InputState& currentState);
		void MouseReleased(int buttonIndex, const InputState& currentState);

	private:
		ID3D10Device*				mDevice;
		Sprite*						mBackground;

		GameFont*					mFont;
		//std::stringstream			mStream;
		std::stringstream			mFirstString;
		std::stringstream			mLastString;
		InputReceiver*				mReceiver;

		bool						mHasFocus;
		bool						mShowMarker;
		float						mMSSinceBlink;
		int							mMaxCharacters;
		
		static const float			C_MARKER_SPEED;
		static const int			C_NUM_VERTICES;
	};
}
#endif
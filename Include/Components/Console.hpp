#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <sstream>
#include <deque>

#include "Buffer.hpp"
#include "Component.hpp"
#include "Effect.hpp"
#include "GameFont.hpp"
#include "GameTime.hpp"
#include "Globals.hpp"
#include "InputField.hpp"
#include "Scrollbar.hpp"


namespace Components
{
	class Console : public Component, public Scrollable, public InputReciever /*: public KeyListener*/
	{
	public:
		Console(ID3D10Device* device, RECT position, D3DXCOLOR bgColor, InputManager* manager, UINT size = 100);
		~Console() throw();

		void Toggle();
		void SetTextColor(D3DXCOLOR newColor);

		// Methods inherited from Component
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		void LostFocus();
		void GotFocus();

		// Methods inherited from Scrollable
		void Scroll(bool isUp);

		// Methods inherited from InputReciever
		void RecieveInput(std::string input);

		/*void KeyPressed(int code);
		void KeyReleased(int code);
		void CharEntered(unsigned char symbol);*/

	private:
		struct TextLine
		{
			std::string				text;
			D3DXCOLOR				color;
		};

		ID3D10Device*				mDevice;
		Effect*						mEffect;
		Buffer*						mVertexBuffer;
		GameFont*					mFont;
		D3DXCOLOR					mTextColor;
		InputField*					mInputField;
		Scrollbar*					mScrollbar;

		bool						mIsToggled;
		std::stringstream			mStream;
		std::deque<TextLine>		mOutput;
		int							mMaxNumRows;
		int							mFirstShowRow;

		const UINT					C_HISTORY_SIZE;

		static const int			C_NUM_VERTICES;

		void CreateBuffer();
		void CreateEffect();
	};
}
#endif
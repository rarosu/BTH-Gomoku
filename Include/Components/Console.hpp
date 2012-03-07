#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <sstream>
#include <deque>
#include "Component.hpp"
#include "ComponentGroup.hpp"
#include "GameFont.hpp"
#include "GameTime.hpp"
#include "Globals.hpp"
#include "InputField.hpp"
#include "Scrollbar.hpp"
#include "Sprite.hpp"

namespace Components
{
	class Console : public ComponentGroup, public Scrollable, public InputReceiver
	{
	public:
		Console(ID3D10Device* device, ComponentGroup* ownerGroup, RECT position, D3DXCOLOR bgColor,
			/*InputSubscription* manager,*/ UINT size = 100);
		~Console() throw();

		void Toggle();
		void SetTextColor(D3DXCOLOR newColor);

		// Methods inherited from Component
		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		void LostFocus();
		void GotFocus();
		// DEBUG
		//virtual std::string GetName();

		// Override method in Component
		void MouseButtonReleased(int index, const InputState& currentState);


		// Methods inherited from Scrollable
		void Scroll(bool isUp);

		// Methods inherited from InputReceiver
		void RecieveInput(std::string input);

	private:
		struct TextLine
		{
			std::string				text;
			D3DXCOLOR				color;
		};

		ID3D10Device*				mDevice;
		GameFont*					mFont;
		D3DXCOLOR					mTextColor;
		InputField*					mInputField;
		Scrollbar*					mScrollbar;
		Sprite*						mBackground;
		D3DXCOLOR					mBGColor;

		//bool						mIsToggled;
		std::stringstream			mStream;
		std::deque<TextLine>		mOutput;
		int							mMaxNumRows;
		int							mFirstShowRow;

		const UINT					C_HISTORY_SIZE;

		static const int			C_NUM_VERTICES;
	};
}
#endif
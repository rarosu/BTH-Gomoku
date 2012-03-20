#ifndef TOGGLEBUTTON_HPP
#define TOGGLEBUTTON_HPP

#include <vector>
#include <string>

#include "Button.hpp"
#include "GameFont.hpp"

namespace Components
{
	typedef std::vector<std::string> StringVector;

	class ToggleButton : public Button
	{
	public:
		ToggleButton(ComponentGroup* ownerGroup, RECT position, const StringVector& stringVector);
		~ToggleButton() throw();
		
		virtual void Initialize(ID3D10Device* device);
		void Draw();
		const std::string& GetCaption() const;

		void SetCurrentValue(int currentValue);
		int GetCurrentValue() const;
		int GetNumberOfValues() const;

		// DEBUG
		std::string GetName();

		bool GetAndResetToggleStatus();
	protected:
		virtual void Refresh(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);

	private:
		GameFont*			mFont;
		D3DXCOLOR			mTextColor;
		StringVector		mValues;
		int					mCurrentValue;

		// If the button has been toggled, and not yet checked externally.
		bool				mHasBeenToggled;	
	};
}
#endif
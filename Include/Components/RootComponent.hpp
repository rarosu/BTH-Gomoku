#ifndef ROOTCOMPONENT_HPP
#define ROOTCOMPONENT_HPP

#include "WinInclude.hpp"
#include "ComponentGroup.hpp"
#include "Console.hpp"

namespace Components
{
	class RootComponent : public ComponentGroup
	{
	public:
		RootComponent(ID3D10Device* device, int width, int height);
		virtual ~RootComponent();

		void Update(GameTime gameTime, const InputState& currInputState, const InputState& prevInputState);
		void Draw();
		void SetFocusedComponent(Component* component);
		bool HasFocus();

		static Console& GetConsole();

	private:
		static RootComponent*		sInstance;

		Console*					mConsole;
	};
}

#endif
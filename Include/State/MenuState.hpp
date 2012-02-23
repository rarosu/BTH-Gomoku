#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "WinInclude.hpp"
#include <vector>

#include "ApplicationState.hpp"
#include "Buffer.hpp"
#include "Effect.hpp"
#include "TextButton.hpp"

namespace State
{
	namespace MenuButton
	{
		enum Button
		{
			Local,
			Network,
			Options,
			Exit,
			Count
		};
	}

	class MenuState : public ApplicationState
	{
	public:
		MenuState(StateID id, ID3D10Device* device, int width, int height);
		~MenuState() throw();
		
		
		// Inherited from Application State
		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();
		void OnStatePushed();
		void OnStatePopped();

	private:
		struct bgVertex
		{
			D3DXVECTOR2		position;
			D3DXVECTOR2		uv;
		};

		ID3D10Device*							mDevice;
		Effect*									mEffect;
		VertexBuffer*							mBuffer;
		std::vector<Components::TextButton*>	mButtons;

		void CreateBuffer(float width, float height);
		void CreateEffect();
	};
}

#endif
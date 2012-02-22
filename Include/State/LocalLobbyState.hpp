#ifndef LOCAL_STATE_HPP
#define LOCAL_STATE_HPP

#include "ApplicationState.hpp"
#include "Globals.hpp"
#include "Label.hpp"
#include "TextButton.hpp"

namespace State
{
	namespace LLobbyButton
	{
		enum Button
		{
			Game,
			Back,
			Count
		};
	}
	class LocalLobbyState : public ApplicationState
	{
	public:
		LocalLobbyState(StateID id, ID3D10Device* device, InputSubscription* manager, int width, int height);
		~LocalLobbyState() throw();

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
		Buffer*									mBuffer;
		std::vector<Components::TextButton*>	mButtons;
		Components::Label*						mTitle;

		void CreateBuffer(float width, float height);
		void CreateEffect();

	};
}

#endif
#ifndef CLIENT_LOBBY_STATE_HPP
#define CLIENT_LOBBY_STATE_HPP

#include "ApplicationState.hpp"
#include "TextButton.hpp"
#include "Label.hpp"
#include "Effect.hpp"
#include "Buffer.hpp"
#include "ClientSession.hpp"

namespace State
{
	class ClientLobbyState : public ApplicationState
	{
	public:
		ClientLobbyState(StateID id, ID3D10Device* device);
		~ClientLobbyState() throw();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();

		void OnStatePushed();
		void OnStatePopped();

		void SetSession(Logic::ClientSession* session);
	private:
		struct bgVertex
		{
			D3DXVECTOR2		position;
			D3DXVECTOR2		uv;
		};

		ID3D10Device*							mDevice;
		Effect*									mEffect;
		VertexBuffer*							mBuffer;
		Components::ComponentGroup*				mComponents;
		std::vector<Components::TextButton*>	mButtons;
		std::vector<Components::Label*>			mPlayerLabels;

		Logic::ClientSession*					mClientSession;

		void CreateComponents();
		void CreateBuffer(float width, float height);
		void CreateEffect();
	};
}
#endif
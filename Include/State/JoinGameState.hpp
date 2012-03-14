#ifndef JOIN_GAME_STATE_HPP
#define JOIN_GAME_STATE_HPP

#include "WinInclude.hpp"
#include "ApplicationState.hpp"
#include "Sprite.hpp"
#include "InputField.hpp"
#include "TextButton.hpp"
#include "Client.hpp"
#include "ClientLobbyState.hpp"

namespace State
{
	class JoinGameState : public ApplicationState
	{
	public:
		JoinGameState(StateID id, ID3D10Device* device, ClientLobbyState* lobbyState);
		~JoinGameState() throw();
		
		void OnStatePushed();
		void OnStatePopped();

		void Update(const InputState& currInput, const InputState& prevInput, const GameTime& gameTime);
		void Draw();
	private:
		struct bgVertex
		{
			D3DXVECTOR2		position;
			D3DXVECTOR2		uv;
		};

		static const int C_CONNECT_STATE_IDLE = 0;
		static const int C_CONNECT_STATE_INVALID = 1;
		static const int C_CONNECT_STATE_CONNECTING = 2;

		ID3D10Device*				mDevice;
		GameFont*					mDefaultFont;
		Sprite*						mBackground;

		Components::ComponentGroup*	mComponents;

		Components::InputField*		mNameField;
		Components::InputField*		mIPAddressField;
		Components::InputField*		mPortField;
		Components::TextButton*		mJoinButton;
		Components::TextButton*		mCancelButton;

		ClientLobbyState*			mClientLobbyState;
		Network::Client*			mClient;

		int							mConnectState;

		void CreateComponents();
	};
}


#endif
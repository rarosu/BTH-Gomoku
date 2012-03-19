#ifndef GAME_HPP
#define GAME_HPP

#include "D3DApplication.hpp"
#include "InputManager.hpp"
#include "GameTime.hpp"
#include "GameFont.hpp"
#include "Console.hpp"
#include "RootComponent.hpp"

#include "MenuState.hpp"
#include "JoinGameState.hpp"
#include "CreateGameState.hpp"
#include "ServerLobbyState.hpp"
#include "ClientLobbyState.hpp"
#include "InGameState.hpp"

class Game : public D3DApplication, public State::StateManager
{
public:
	Game(HINSTANCE applicationInstance, LPCTSTR windowTitle = "GameWindow", 
		UINT clientWidth = 1024, UINT clientHeight = 576);
	~Game() throw();

	void Update();
	void Draw();
	LRESULT HandleAppMessages(UINT message, WPARAM wParam, LPARAM lParam);

	/**
		Implemented StateManager
	*/
	void ChangeState(State::ApplicationState* state);
private:
	GameTime					mGameTime;
	GameFont*					mDefaultFont;
	InputManager				mInputManager;
	SoundManager				mSoundManager;
	Components::RootComponent*	mRootComponentGroup;

	State::MenuState*			mMenuState;
	State::CreateGameState*		mCreateGameState;
	State::JoinGameState*		mJoinGameState;
	State::ServerLobbyState*	mServerLobbyState;
	State::ClientLobbyState*	mClientLobbyState;
	State::InGameState*			mInGameState;

	State::ApplicationState*	mCurrentState;
	State::ApplicationState*	mNextState;

protected:
	void ProgramLoop();
	void OnResize();
};
#endif
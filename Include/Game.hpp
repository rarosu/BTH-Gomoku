#ifndef GAME_HPP
#define GAME_HPP

#include "D3DApplication.hpp"
#include "InputManager.hpp"
#include "GameTime.hpp"
#include "GameFont.hpp"
#include "Console.hpp"
#include "RootComponent.hpp"

#include "MenuState.hpp"
#include "LobbyState.hpp"
#include "InGameState.hpp"
#include "JoinGameState.hpp"
#include "CreateGameState.hpp"

class Game : public D3DApplication
{
public:
	Game(HINSTANCE applicationInstance, LPCTSTR windowTitle = "GameWindow", 
		UINT clientWidth = 640, UINT clientHeight = 480);
	~Game() throw();

	void Update();
	void Draw();
	LRESULT HandleAppMessages(UINT message, WPARAM wParam, LPARAM lParam);

private:
	GameTime					mGameTime;
	GameFont*					mDefaultFont;
	InputManager				mInputManager;
	Components::RootComponent*	mRootComponentGroup;

	State::MenuState*			mMenuState;
	State::InGameState*			mInGameState;
	State::LobbyState*			mLobbyState;
	State::CreateGameState*		mCreateGameState;
	State::JoinGameState*		mJoinGameState;
protected:
	void ProgramLoop();
	void OnResize();
};
#endif
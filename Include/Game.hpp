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
#include "CreateAGameState.hpp"

class Game : public D3DApplication
{
public:
	Game(HINSTANCE applicationInstance, LPCTSTR windowTitle = "GameWindow", 
		UINT clientWidth = 1024, UINT clientHeight = 576);
	~Game() throw();

	void Update();
	void Draw();
	LRESULT HandleAppMessages(UINT message, WPARAM wParam, LPARAM lParam);

private:
	GameTime					mGameTime;
	GameFont*					mDefaultFont;
	//Components::Console*		mConsole;				// DEBUG: will be removed
	InputManager				mInputManager;
	Components::RootComponent*	mRootComponentGroup;

	State::MenuState*			mMenuState;
	State::InGameState*			mInGameState;
	State::LobbyState*			mLobbyState;
	State::CreateAGameState*	mCreateGameState;

protected:
	void ProgramLoop();
	void OnResize();
};
#endif
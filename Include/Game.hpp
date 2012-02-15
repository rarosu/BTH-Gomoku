#ifndef GAME_HPP
#define GAME_HPP

#include "D3DApplication.hpp"
#include "InputManager.hpp"
#include "GameTime.hpp"
#include "GameFont.hpp"
#include "Console.hpp"

#include "MenuState.hpp"
#include "LocalLobbyState.hpp"
#include "InGameState.hpp"

class Game : public D3DApplication
{
public:
	Game(HINSTANCE applicationInstance, LPCTSTR windowTitle = "GameWindow", 
		UINT windowWidth = CW_USEDEFAULT, UINT windowHeight = CW_USEDEFAULT);
	~Game() throw();

	void Update();
	void Draw();
	LRESULT HandleAppMessages(UINT message, WPARAM wParam, LPARAM lParam);

private:
	GameTime					mGameTime;
	GameFont*					mDefaultFont;
	Components::Console*		mConsole;
	InputManager				mInputManager;

	State::MenuState*			mMenuState;
	State::LocalLobbyState*		mLocalLobbyState;
	State::InGameState*			mInGameState;

	Frustrum					mViewFrustrum;

protected:
	void ProgramLoop();
	void OnResize();
};
#endif
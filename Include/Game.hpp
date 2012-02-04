#ifndef GAME_HPP
#define GAME_HPP

#include "D3DApplication.hpp"
#include "InputManager.hpp"
#include "GameTime.hpp"
#include "GameFont.hpp"
#include "Console.hpp"
#include "Camera.hpp"
#include "Logic/Grid.hpp"
#include "Scene.hpp"

class Game : public D3DApplication
{
public:
	Game(HINSTANCE applicationInstance, LPCTSTR windowTitle = "GameWindow", 
		UINT windowWidth = CW_USEDEFAULT, UINT windowHeight = CW_USEDEFAULT);
	virtual ~Game();
	virtual void Update();
	virtual void Draw();
	virtual LRESULT HandleAppMessages(UINT message, WPARAM wParam, LPARAM lParam);

private:
	GameTime		mGameTime;
	GameFont*		mDefaultFont;
	Console*		mConsole;
	InputManager	mInputManager;
	Scene*			mScene;
	Camera*			mCamera;

	Logic::Grid*	mGrid;
protected:
	virtual void ProgramLoop();
};
#endif
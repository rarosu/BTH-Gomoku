#ifndef GAME_HPP
#define GAME_HPP

#include "D3DApplication.hpp"
#include "StandardButton.hpp"
#include "InputManager.hpp"
#include "InputField.hpp"
#include "GameTime.hpp"
#include "GameFont.hpp"
#include "Console.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "Marker.hpp"
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
	GameTime			mGameTime;
	GameFont*			mDefaultFont;
	Console*			mConsole;
	InputManager		mInputManager;
	Scene*				mScene;
	Camera*				mCamera;
	Marker*				mMarker;

	Logic::Grid*	mGrid;
protected:
	virtual void ProgramLoop();
};
#endif
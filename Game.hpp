#ifndef GAME_H
#define GAME_H

#include "D3DApplication.hpp"
#include "GameTime.hpp"
#include "GameFont.hpp"

class Game : public D3DApplication
{
public:
	Game(HINSTANCE applicationInstance, LPCTSTR windowTitle = "GameWindow", 
		UINT windowWidth = CW_USEDEFAULT, UINT windowHeight = CW_USEDEFAULT);
	virtual ~Game();
	virtual void Update();
	virtual void Draw();

private:
	GameTime mGameTime;
	GameFont* mDefaultFont;

protected:
	virtual void ProgramLoop();
};
#endif
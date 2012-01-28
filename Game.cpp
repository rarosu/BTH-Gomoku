#include "Game.hpp"

Game::Game(HINSTANCE applicationInstance, LPCTSTR windowTitle, UINT windowWidth, UINT windowHeight)
	: D3DApplication(applicationInstance, windowTitle, windowWidth, windowHeight), mGameTime()
{
	mDefaultFont = new GameFont(mDeviceD3D, "Times New Roman", 24);
	mConsole = new Console(mDeviceD3D, (float)mScreenWidth, (float)mScreenHeight);
}

Game::~Game()
{
}

//  What happens every loop of the program (ie updating and drawing the game)
void Game::ProgramLoop()
{
	Update();
	Draw();
}

// Update the game
void Game::Update()
{
	mGameTime.Update();
}

// Draw the scene
void Game::Draw()
{
	ClearScene();
	
	POINT position = { 50, 50 };
	const D3DXCOLOR RED(1.0, 0.0, 0.0, 1.0);

	mDefaultFont->WriteText(L"Hello World!", position, RED);

	mConsole->Draw();

	RenderScene();
}
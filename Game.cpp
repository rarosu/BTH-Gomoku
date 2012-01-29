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
	mConsole->Update(mGameTime);

	if(GetAsyncKeyState(VK_ESCAPE))
		Quit();
	if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState('T'))
	{
		mConsole->Toggle();
	}
}

// Draw the scene
void Game::Draw()
{
	ClearScene();

	mConsole->Draw();

	RenderScene();
}

LRESULT Game::HandleAppMessages(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CHAR:
			mConsole->KeyPressed((unsigned char)wParam);
			return 0;
	}

	return D3DApplication::HandleAppMessages(message, wParam, lParam);
}
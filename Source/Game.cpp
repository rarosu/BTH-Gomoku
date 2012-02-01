#include "Game.hpp"

Game::Game(HINSTANCE applicationInstance, LPCTSTR windowTitle, UINT windowWidth, UINT windowHeight) : 
	D3DApplication(applicationInstance, windowTitle, windowWidth, windowHeight)
{
	mDefaultFont = new GameFont(mDeviceD3D, "Times New Roman", 24);
	mConsole = new Console(mDeviceD3D, (float)mScreenWidth, (float)mScreenHeight);
	mGrid = new Logic::Grid();
	mInputManager.AddKeyListener(mConsole);
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

	mInputManager.Update();
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
			mInputManager.HandleCharPress(wParam, lParam);
			return 0;
		case WM_KEYDOWN:
			mInputManager.HandleKeyPress(wParam, lParam);
			return 0;
		case WM_KEYUP:
			mInputManager.HandleKeyRelease(wParam, lParam);
			return 0;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			mInputManager.HandleMousePress(wParam, lParam);
			return 0;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			mInputManager.HandleMouseRelease(wParam, lParam);
			return 0;
		case WM_MOUSEMOVE:
			mInputManager.HandleMouseMove(wParam, lParam);
			return 0;
	}

	return D3DApplication::HandleAppMessages(message, wParam, lParam);
}
#include "Game.hpp"

Game::Game(HINSTANCE applicationInstance, LPCTSTR windowTitle, UINT windowWidth, UINT windowHeight) : 
	D3DApplication(applicationInstance, windowTitle, windowWidth, windowHeight),
	mMenuState(NULL),
	mLocalLobbyState(NULL),
	mInGameState(NULL),
	mDefaultFont(NULL),
	mConsole(NULL)
{
	UpdateViewportMatrix(mScreenWidth, mScreenHeight);

	mDefaultFont = new GameFont(mDeviceD3D, "Times New Roman", 24);
	RECT consolePos = { 0, 0, mScreenWidth, mScreenHeight / 2 };
	mConsole = new Console(mDeviceD3D, consolePos, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), &mInputManager);

	mViewFrustrum.nearDistance =	1.0f;
	mViewFrustrum.farDistance =		1000.0f;
	mViewFrustrum.fovY =			(float)D3DX_PI * 0.25f;
	mViewFrustrum.aspectRatio =		(float) mScreenWidth / (float) mScreenHeight;

	mMenuState = new State::MenuState(State::C_STATE_MENU);
	mLocalLobbyState = new State::LocalLobbyState(State::C_STATE_LOCAL_LOBBY);
	mInGameState = new State::InGameState(State::C_STATE_IN_GAME, mDeviceD3D, mViewFrustrum);

	// Start the application in InGameState
	State::ApplicationState::sStack.ChangeState(mInGameState);
	State::ApplicationState::sStack.UpdateStack();
}

Game::~Game()
{
	SafeDelete(mConsole);
	SafeDelete(mDefaultFont);

	SafeDelete(mMenuState);
	SafeDelete(mLocalLobbyState);
	SafeDelete(mInGameState);
}

//  What happens every loop of the program (ie updating and drawing the game)
void Game::ProgramLoop()
{
	// Make sure the next state is valid - if a NULL state has been specified,
	// the program will exit
	if (State::ApplicationState::sStack.GetNextState() != NULL)
	{
		Update();
		Draw();
	}
	else
	{
		Quit();
	}
}

// Update the game
void Game::Update()
{
	// Update the state stack
	State::ApplicationState::sStack.UpdateStack();

	// Update game time, input and console
	mGameTime.Update();
	mInputManager.Update();
	mConsole->Update(mGameTime);

	// Update the topmost state
	State::ApplicationState::sStack.UpdateState(mInputManager, mGameTime);



	/**
		TODO: Change this to use input manager instead of async checks
	*/
	/*
	if(GetAsyncKeyState(VK_ESCAPE))
		Quit();
	if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState('T'))
	{
		mConsole->Toggle();
	}
	if(GetAsyncKeyState(VK_F7))
		mConsole->SetTextColor(D3DXCOLOR(1.0, 0.0, 0.0, 1.0));
	if(GetAsyncKeyState(VK_F8))
		mConsole->SetTextColor(D3DXCOLOR(0.0, 0.0, 0.0, 1.0));
	*/
	/**
	*/
}

// Draw the scene
void Game::Draw()
{
	ClearScene();
	
	// Draw the topmost state
	State::ApplicationState::sStack.DrawState();

	// Draw the console on top of everything else
	mConsole->Draw();
	
	// Swap backbuffer
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
			mInputManager.HandleMouseRelease(C_MOUSE_LEFT, wParam, lParam);
			return 0;
		case WM_RBUTTONUP:
			mInputManager.HandleMouseRelease(C_MOUSE_RIGHT, wParam, lParam);
			return 0;
		case WM_MBUTTONUP:
			mInputManager.HandleMouseRelease(C_MOUSE_MIDDLE, wParam, lParam);
			return 0;
		case WM_MOUSEMOVE:
			mInputManager.HandleMouseMove(wParam, lParam);
			return 0;
		case WM_MOUSEWHEEL:
			mInputManager.HandleMouseWheel(wParam, lParam);
			return 0;
	}

	return D3DApplication::HandleAppMessages(message, wParam, lParam);
}

void Game::OnResize()
{
	D3DApplication::OnResize();

	// TODO: forward to states
}
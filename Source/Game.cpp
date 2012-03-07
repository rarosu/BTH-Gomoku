#include "Game.hpp"

Game::Game(HINSTANCE applicationInstance, LPCTSTR windowTitle, UINT clientWidth, UINT clientHeight) : 
	D3DApplication(applicationInstance, windowTitle, clientWidth, clientHeight),
	mCurrentState(NULL),
	mNextState(NULL),
	mMenuState(NULL),
	mInGameState(NULL),
	mCreateGameState(NULL),
	mJoinGameState(NULL),
	mDefaultFont(NULL),
	mRootComponentGroup(NULL)
{
	// Create general objects
	mDefaultFont = new GameFont(mDeviceD3D, "Times New Roman", 24);

	// Setup static access variables
	Components::Component::sViewport = &mViewport;
	State::ApplicationState::sViewport = &mViewport;
	State::ApplicationState::sInputManager = &mInputManager;
	State::ApplicationState::sManager = this;

	mRootComponentGroup = new Components::RootComponent(mDeviceD3D, mViewport.GetWidth(), mViewport.GetHeight());
	mInputManager.AddKeyListener(mRootComponentGroup);
	mInputManager.AddMouseListener(mRootComponentGroup);
	State::ApplicationState::sRootComponentGroup = mRootComponentGroup;

	// Create the states
	mMenuState = new State::MenuState(State::C_STATE_MENU, mDeviceD3D);
	mServerLobbyState = new State::ServerLobbyState(State::C_STATE_SERVER_LOBBY, mDeviceD3D);
	mClientLobbyState = new State::ClientLobbyState(State::C_STATE_CLIENT_LOBBY, mDeviceD3D);
	mCreateGameState = new State::CreateGameState(State::C_STATE_CREATE_GAME, mDeviceD3D, mServerLobbyState);
	mJoinGameState = new State::JoinGameState(State::C_STATE_JOIN_GAME, mDeviceD3D);
	mInGameState = new State::InGameState(State::C_STATE_IN_GAME, mDeviceD3D);

	// Set the starting state
	mCurrentState = mMenuState;
	mNextState = mMenuState;
	mCurrentState->OnStatePushed();
}


Game::~Game()
{
	// If the current state isn't NULL, notify it that it is being popped before destroying it.
	if (mCurrentState != NULL)
		mCurrentState->OnStatePopped();

	SafeDelete(mDefaultFont);

	SafeDelete(mMenuState);
	SafeDelete(mCreateGameState);
	SafeDelete(mJoinGameState);
	SafeDelete(mServerLobbyState);
	SafeDelete(mInGameState);

	SafeDelete(mRootComponentGroup);
}

//  What happens every loop of the program (ie updating and drawing the game)
void Game::ProgramLoop()
{
	// Swap to the next state, if the state has been changed
	if (mNextState != mCurrentState)
	{
		if (mNextState != NULL)		mNextState->OnStatePushed();
		if (mCurrentState != NULL)	mCurrentState->OnStatePopped();

		mCurrentState = mNextState;
	}
	
	// If the state has been changed to NULL, we're supposed to quit
	if (mCurrentState != NULL)
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
	// Update game time and GUI components
	mGameTime.Update();
	mRootComponentGroup->Update(mGameTime, mInputManager.GetCurrent(), mInputManager.GetPrevious());

	// Update the current state
	mCurrentState->Update(mInputManager.GetCurrent(), mInputManager.GetPrevious(), mGameTime);

	// Update the input
	mInputManager.Update();
}

// Draw the scene
void Game::Draw()
{
	ClearScene();
	


	// Draw the current state
	mCurrentState->Draw();

	// Draw the HUD
	mRootComponentGroup->Draw();

	// DEBUG - Output the name of the component that currently has focus
	POINT pos = { 10, mViewport.GetHeight() - 40};
	mDefaultFont->WriteText(mRootComponentGroup->GetName(), pos, D3DXCOLOR(1.0, 0.0, 0.0, 1.0));
	// /DEBUG


	
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

void Game::ChangeState(State::ApplicationState* state)
{
	mNextState = state;
}

void Game::OnResize()
{
	D3DApplication::OnResize();

	if (mCurrentState != NULL)
		mCurrentState->OnResize();
}
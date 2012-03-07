#include "Game.hpp"

Game::Game(HINSTANCE applicationInstance, LPCTSTR windowTitle, UINT clientWidth, UINT clientHeight) : 
	D3DApplication(applicationInstance, windowTitle, clientWidth, clientHeight),
	mMenuState(NULL),
	mInGameState(NULL),
	mDefaultFont(NULL),
	mRootComponentGroup(NULL)
{
	// Create general objects
	mDefaultFont = new GameFont(mDeviceD3D, "Times New Roman", 24);
	/*RECT consolePos = { 0, 0, mViewport.GetWidth(), mViewport.GetHeight() / 2 };
	mConsole = new Components::Console(mDeviceD3D, mRootComponentGroup, consolePos, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));*/
	//mRootComponentGroup->SetFocusedComponent(mConsole);

	// Setup static access variables
	Components::Component::sViewport = &mViewport;
	State::ApplicationState::sViewport = &mViewport;
	State::ApplicationState::sInputManager = &mInputManager;

	mRootComponentGroup = new Components::RootComponent(mDeviceD3D, mViewport.GetWidth(), mViewport.GetHeight());
	mInputManager.AddKeyListener(mRootComponentGroup);
	mInputManager.AddMouseListener(mRootComponentGroup);
	State::ApplicationState::sRootComponentGroup = mRootComponentGroup;

	// Create the states
	mMenuState = new State::MenuState(State::C_STATE_MENU, mDeviceD3D);
	mLobbyState = new State::LobbyState(State::C_STATE_LOBBY, mDeviceD3D);
	mInGameState = new State::InGameState(State::C_STATE_IN_GAME, mDeviceD3D);
	mCreateGameState = new State::CreateAGameState(State::C_STATE_CREATE_GAME, mDeviceD3D);

	// Start the application in InGameState
	State::ApplicationState::sStack.ChangeState(mInGameState);
	State::ApplicationState::sStack.UpdateStack();
}


Game::~Game()
{
	SafeDelete(mDefaultFont);

	SafeDelete(mMenuState);
	SafeDelete(mInGameState);
	SafeDelete(mLobbyState);

	SafeDelete(mCreateGameState);

	SafeDelete(mRootComponentGroup);
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

	// Update game time and GUI components
	mGameTime.Update();
	mRootComponentGroup->Update(mGameTime, mInputManager.GetCurrent(), mInputManager.GetPrevious());

	// Update the topmost state
	State::ApplicationState::sStack.UpdateState(mInputManager.GetCurrent(), 
		mInputManager.GetPrevious(), mGameTime);

	mInputManager.Update();					// Keep last
}

// Draw the scene
void Game::Draw()
{
	ClearScene();
	
	// Draw the topmost state
	State::ApplicationState::sStack.DrawState();
	// Draw the console on top of everything else
	//mConsole->Draw();
	mRootComponentGroup->Draw();

	POINT pos = { 10, mViewport.GetHeight() - 40};
	mDefaultFont->WriteText(mRootComponentGroup->GetName(), pos, D3DXCOLOR(1.0, 0.0, 0.0, 1.0));
	
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

	if (State::ApplicationState::sStack.Top() != NULL)
		State::ApplicationState::sStack.Top()->OnResize();
}
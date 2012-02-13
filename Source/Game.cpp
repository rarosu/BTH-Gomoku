#include "Game.hpp"

Game::Game(HINSTANCE applicationInstance, LPCTSTR windowTitle, UINT windowWidth, UINT windowHeight) : 
	D3DApplication(applicationInstance, windowTitle, windowWidth, windowHeight)
{
	UpdateViewportMatrix(mScreenWidth, mScreenHeight);

	mDefaultFont = new GameFont(mDeviceD3D, "Times New Roman", 24);
	RECT consolePos = { 0, 0, mScreenWidth, mScreenHeight / 2 };
	mConsole = new Console(mDeviceD3D, consolePos, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f),
		&mInputManager);
	mGrid = new Logic::Grid();
	mScene = new Scene(mDeviceD3D);
	mMarker = new Marker(mDeviceD3D, 6, D3DXVECTOR3(5, 1, 5));
	
	Frustrum viewFrustrum;
	viewFrustrum.nearDistance = 1.0f;
	viewFrustrum.farDistance = 1000.0f;
	viewFrustrum.fovY = (float)D3DX_PI * 0.25f;
	viewFrustrum.aspectRatio = (float) mScreenWidth / (float) mScreenHeight;
	mCamera = new Camera(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, -1.0f, 2.0f), D3DXVECTOR3(0, 1.0f, 0), viewFrustrum);

	//mInputManager.AddKeyListener(mConsole);
	mInputManager.AddMouseListener(mCamera);
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
	mCamera->Update(mInputManager.GetPrevious(), mInputManager.GetCurrent(), mGameTime);
	mConsole->Update(mGameTime);
	mScene->Update(mGrid, *mCamera);
	mMarker->Update(*mCamera);

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

	mInputManager.Update();
}

// Draw the scene
void Game::Draw()
{
	ClearScene();
	
	mScene->Draw();
	mMarker->Draw();
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
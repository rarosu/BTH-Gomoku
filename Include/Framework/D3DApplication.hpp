#ifndef D3DAPPLICATION_HPP
#define D3DAPPLICATION_HPP

#include "Globals.hpp"
#include "Viewport.hpp"


class D3DApplication
{
public:
	D3DApplication(HINSTANCE applicationInstance, LPCTSTR windowTitle = "ApplicationWindow", 
	UINT clientWidth = 640, UINT clientHeight = 480);
	virtual ~D3DApplication();
	int Run();
	virtual LRESULT HandleAppMessages(UINT message, WPARAM wParam, LPARAM lParam);
	
private:
	// Window variables
	HWND					mWindowHandle;

	// Direct3D variables
	IDXGISwapChain*			mSwapChain;
	ID3D10RenderTargetView*	mRenderTarget;
	ID3D10Texture2D*		mDepthStencilBuffer;
	ID3D10DepthStencilView* mDepthStencilView;
	D3DXCOLOR				mClearColor;

	// Methods
	D3DApplication(); // Default contstructor is private, the other constructor must be used
	bool InitWindowsApplication(HINSTANCE applicationHandle, LPCTSTR title, int showSetting);
	HRESULT InitializeDirect3D();
	/*void CalculateWindowSize();*/
	HRESULT SetUpView();
	HRESULT CreateSwapChain();
	HRESULT CreateRenderTargetView();
	HRESULT CreateDepthStencil();
	/*void SetUpViewport();*/

protected:
	ID3D10Device*			mDeviceD3D;
	Viewport				mViewport;

	void ShowErrorMessage(LPCSTR message);
	virtual void ProgramLoop();
	void ClearScene();
	void RenderScene();
	virtual void OnResize();
	void Quit();
};
#endif
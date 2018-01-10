#include "Core.h"
#include "Helpers\LightTypes.h"
#include "Helpers\EngineDefines.h"

#include "DirectX\DxResourcesManager.h"

namespace aqua
{

Core* patterns::Singleton<Core>::s_instance = nullptr;

bool Core::Initialize()
{
	int2 windowSz = InitializeWindows();
	//Add assertion code
	DxResourcesManager::CreateInstance();
	DxResourcesManager::GetInstance()->Initialize(TEST_SCREEN_WIDTH, TEST_SCREEN_HEIGHT, true, m_hwnd,
		false, 1000.0f, 0.1f);

	return true;
}

bool Core::Shutdown()
{
	ShutdownWindows();

	m_hwnd = nullptr;
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = nullptr;

	return true;
}

LRESULT Core::HandleMessage(HWND _hwnd, const UINT _umsg, WPARAM _wparam, LPARAM _lparam)
{
	switch (_umsg) 
	{
	/*case WM_KEYDOWN:
		mInput->KeyDown((unsigned int)_wparam);
		return 0;*/

		/* case WM_KEYUP:
		mInput->KeyUp((unsigned int)_wparam);
		return 0;*/

	default:
		return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
	}
}

void Core::RunMainLoop()
{
	MSG msg;
	bool done;
	bool result;

	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) 
		{
			done = true;
		}
		else 
		{
			/*result = ProcessFrame();
			if (!result) {
				done = true;
			}*/
			float color[4] = { 0.0, 1.0, 0.0, 1.0 };
			DxResourcesManager::GetInstance()->BeginScene(color);
			DxResourcesManager::GetInstance()->EndScene();

		}
	}
}

int2 Core::InitializeWindows()
{
	int2 windowSz;
	WNDCLASSEX wc;

	// Get the instance of this application.
	m_hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Aqua Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	windowSz.x = GetSystemMetrics(SM_CXSCREEN);
	windowSz.y = GetSystemMetrics(SM_CYSCREEN);

	int posX;
	int posY;
	
#ifdef FULL_SCREEN
	DEVMODE dmScreenSettings;
	// If full screen set the screen to maximum size of the users desktop and 32bit.
	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth = (unsigned long)windowSz.x;
	dmScreenSettings.dmPelsHeight = (unsigned long)windowSz.y;
	dmScreenSettings.dmBitsPerPel = 32;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	// Change the display settings to full screen.
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

	posX = posY = 0;
#else
	windowSz.x = TEST_SCREEN_WIDTH;
	windowSz.y = TEST_SCREEN_HEIGHT;

	posX = (GetSystemMetrics(SM_CXSCREEN) - windowSz.x) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - windowSz.y) / 2;
#endif

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, windowSz.x, windowSz.y, NULL, NULL, m_hInstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);

	return	windowSz;
}

void Core::ShutdownWindows()
{
	ShowCursor(true);

#ifdef FULL_SCREEN
	ChangeDisplaySettings(NULL, 0);
#endif

	DestroyWindow(m_hwnd);
}

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _umessage, WPARAM _wparam, LPARAM _lparam)
{
	switch (_umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
		return aqua::Core::GetInstance()->HandleMessage(_hwnd, _umessage, _wparam, _lparam);

	}
}

}
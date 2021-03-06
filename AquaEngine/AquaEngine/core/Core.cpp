#include "StdAfx.h"
#include "Core.h"

#include "Helpers\LightTypes.h"
#include "Helpers\EngineDefines.h"

#include "DirectX\DxResourcesManager.h"

#include "EntityComponentSystem\Components\ComponentPool.h"
#include "EntityComponentSystem\ECSManager.h"

#include "Input\InputMgr.h"

namespace aqua
{

bool Core::Initialize()
{
	int2 windowSz = InitializeWindows();
	//Add assertion code
	DxResourcesManager::CreateInstance();
	DxResourcesManager::GetInstance()->Initialize(Defines::k_screenWidth, Defines::k_screenHeight, true, m_hwnd,
		false, 1000.0f, 0.1f);

	ecs::ComponentPool::CreateInstance();
	ecs::ComponentPool::GetInstance()->Initialize();

	ecs::ECSManager::CreateInstance();
	ecs::ECSManager::GetInstance()->Initialize();
	ecs::ECSManager::GetInstance()->LoadEntities();
	ecs::ECSManager::GetInstance()->LoadSystems();

	InputMgr::CreateInstance();
	InputMgr::GetInstance()->Initialize();

	return true;
}

bool Core::Shutdown()
{
	InputMgr::GetInstance()->Shutdown();
	ecs::ECSManager::GetInstance()->Shutdown();
	DxResourcesManager::GetInstance()->Shutdown();
	ecs::ComponentPool::GetInstance()->Shutdown();

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
	case WM_KEYDOWN:
		InputMgr::GetInstance()->KeyDown((unsigned int)_wparam);
		return 0;
		break;

	default:
		return DefWindowProc(_hwnd, _umsg, _wparam, _lparam);
	}
}

void Core::RunMainLoop()
{
	MSG msg;
	bool done;

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
			done = !ProcessFrame();
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
	
	if (Defines::k_fullScreen)
	{
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
	}
	else
	{
		windowSz.x = Defines::k_screenWidth;
		windowSz.y = Defines::k_screenHeight;

		posX = (GetSystemMetrics(SM_CXSCREEN) - windowSz.x) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - windowSz.y) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, windowSz.x, windowSz.y, NULL, NULL, m_hInstance, NULL);

	::vs_log(m_hwnd != nullptr, ERROR_msg, L"Failed to create window, return code : ", GetLastError());

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

bool Core::ProcessFrame()
{
	if (InputMgr::GetInstance()->GetLastPressed() == VK_ESCAPE)
	{
		return false;
	}

	float color[4] = { 0.0, 0.0, 0.0, 1.0 };
	DxResourcesManager::GetInstance()->BeginScene(color);
	ecs::ECSManager::GetInstance()->Update();
	DxResourcesManager::GetInstance()->EndScene();

	return true;
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
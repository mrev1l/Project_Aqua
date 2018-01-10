#pragma once
#define WIN32_LEAN_AND_MEAN

#include "Patterns\Singleton.h"

#include <Windows.h>

namespace aqua
{
struct int2;

class Core : public patterns::Singleton<Core>
{
public:
	bool Initialize() override;
	bool Shutdown() override;

	LRESULT CALLBACK HandleMessage(HWND _hwnd, const UINT _umsg, WPARAM _wparam, LPARAM _lparam);

	void RunMainLoop();

private:
	int2 InitializeWindows();
	void ShutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;
};

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _umessage, WPARAM _wparam, LPARAM _lparam);

}

#include "StdAfx.h"

#include "core\Core.h"

#include <memory>

#pragma warning(push)
#pragma warning(disable:4100)

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _pScmdline, int _iCmdshow) 
{
	aqua::Core::CreateInstance();
	if (!aqua::Core::GetInstance()->Initialize())
	{
		vs_log(true, ERROR_msg, L"Aqua Engine failed to initialize!!!");
		return -1;
	}

	aqua::Core::GetInstance()->RunMainLoop();

	aqua::Core::GetInstance()->Shutdown();

	return 0;
}

#pragma warning(pop)

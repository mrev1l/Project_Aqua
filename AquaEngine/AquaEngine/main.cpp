#include "core\Core.h"
#include <memory>

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _pScmdline, int _iCmdshow) 
{
	aqua::Core::CreateInstance();
	if (!aqua::Core::GetInstance()->Initialize())
	{
		return -1;
	}

	aqua::Core::GetInstance()->RunMainLoop();

	return 0;
}
#pragma once
#include "core\Patterns\Singleton.h"

namespace aqua
{

class InputMgr : public patterns::Singleton<InputMgr>
{
public:
	bool Initialize() override;
	bool Shutdown() override;

	void KeyDown(unsigned int _key);

	unsigned int GetLastPressed();

private:
	unsigned int m_lastPressed;
};

}

#include "InputMgr.h"

namespace aqua
{

bool InputMgr::Initialize()
{
	m_lastPressed = 0U;
	return true;
}

bool InputMgr::Shutdown()
{
	return false;
}

void InputMgr::KeyDown(unsigned int _key)
{
	m_lastPressed = _key;
}

unsigned int InputMgr::GetLastPressed()
{
	return m_lastPressed;
}

}

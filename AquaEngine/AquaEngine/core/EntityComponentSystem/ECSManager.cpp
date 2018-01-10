#include "ECSManager.h"

namespace aqua
{

ecs::ECSManager* patterns::Singleton<ecs::ECSManager>::s_instance = nullptr;

namespace ecs
{


bool ECSManager::Initialize()
{
	return false;
}

bool ECSManager::Shutdown()
{
	return false;
}

}
}
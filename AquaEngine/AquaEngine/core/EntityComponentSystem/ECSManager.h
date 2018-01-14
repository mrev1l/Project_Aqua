#pragma once
#include "core\Patterns\Singleton.h"

#include <vector>

namespace aqua
{
namespace ecs
{

class BaseSystem;
class BaseEntity;

class ECSManager : public patterns::Singleton<ECSManager>
{
public:
	bool Initialize() override;
	bool Shutdown() override;

	void LoadEntities();
	void LoadSystems();

	void Update();

private:
	std::vector<BaseSystem*> m_systems;
	std::vector<BaseEntity*> m_entities;
};

}
}
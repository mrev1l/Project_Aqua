#pragma once
#include "core\Patterns\Singleton.h"
#include "core\Helpers\Enums.h"

#include <vector>

namespace aqua
{
namespace ecs
{

class BaseComponent;
class BaseNode;

class ComponentPool : public patterns::Singleton<ComponentPool>
{
public:
	bool Initialize() override;
	bool Shutdown() override;

	BaseComponent* CreateComponent(ComponentType _type);
	BaseNode* CreateNode(NodeType _type);

private:
	std::vector<BaseComponent*> m_componentsPool;
	std::vector<BaseNode*> m_nodesPool;
};

}
}
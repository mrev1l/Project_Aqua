#pragma once
#include "core\EntityComponentSystem\Nodes\BaseNode.h"
#include <vector>

namespace aqua
{
namespace ecs
{

class BaseSystem
{
public:
	std::vector<BaseNode*> m_targets;

	void Update();
};

}
}
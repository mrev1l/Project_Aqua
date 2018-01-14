#include "BaseSystem.h"

namespace aqua
{
namespace ecs
{

void BaseSystem::AddTarget(BaseNode * _node)
{
	m_targets.push_back(_node);
}

}
}

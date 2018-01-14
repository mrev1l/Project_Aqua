#include "BaseEntity.h"

namespace aqua
{
namespace ecs
{

void BaseEntity::AddComponent(BaseComponent * _component)
{
	m_components.push_back(_component);
}

}
}
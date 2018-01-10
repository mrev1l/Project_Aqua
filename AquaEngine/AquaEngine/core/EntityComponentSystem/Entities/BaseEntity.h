#pragma once
#include "core\EntityComponentSystem\Components\BaseComponent.h"
#include <vector>

namespace aqua
{
namespace ecs
{

struct BaseEntity
{
	std::vector<BaseComponent*> m_components;
};

}
}
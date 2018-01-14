#pragma once
#include <vector>

namespace aqua
{
namespace ecs
{

class BaseComponent;

class BaseEntity
{
public:
	BaseEntity() = default;
	virtual ~BaseEntity() = default;

	void AddComponent(BaseComponent* _component);

private:
	std::vector<BaseComponent*> m_components;
};

}
}
#pragma once
#include <vector>

namespace aqua
{
namespace ecs
{

class BaseNode;

class BaseSystem
{
public:
	BaseSystem() = default;
	virtual ~BaseSystem() = default;

	void AddTarget(BaseNode* _node);

	virtual void Update() = 0;

protected:
	std::vector<BaseNode*> m_targets;
};

}
}
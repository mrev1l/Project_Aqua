#pragma once
#include "BaseSystem.h"

namespace aqua
{
namespace ecs
{

class RenderableSystem : public BaseSystem
{
public:
	RenderableSystem() = default;
	~RenderableSystem() override = default;

	void Update() override;
};

}
}
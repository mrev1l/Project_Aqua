#pragma once
#include "BaseNode.h"

namespace aqua
{
namespace ecs
{

class ModelComponent;
class LayoutComponent;

class RenderableNode : public BaseNode
{
public:
	RenderableNode() = default;
	~RenderableNode() override = default;

	ModelComponent* m_model;
	LayoutComponent* m_layout;

};

}
}
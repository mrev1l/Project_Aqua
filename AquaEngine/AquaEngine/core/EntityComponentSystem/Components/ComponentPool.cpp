#include "ComponentPool.h"

#include "core\EntityComponentSystem\Components\ModelComponent.h"
#include "core\EntityComponentSystem\Components\LayoutComponent.h"

#include "core\EntityComponentSystem\Nodes\RenderableNode.h"

namespace aqua
{
namespace ecs
{

bool ComponentPool::Initialize()
{
	return true;
}

bool ComponentPool::Shutdown()
{
	return true;
}

BaseComponent* ComponentPool::CreateComponent(ComponentType _type)
{
	switch (_type)
	{
	case Model_Component:
		m_componentsPool.push_back(new ModelComponent());
		break;
	case Layout_Component:
		m_componentsPool.push_back(new LayoutComponent());
		break;
	case Component_Type_Count:
	default:
		// assert
		break;
	}

	m_componentsPool.back()->InitializeComponent();

	return m_componentsPool.back();
}

BaseNode * ComponentPool::CreateNode(NodeType _type)
{
	switch (_type)
	{
	case Renderable_Node:
		m_nodesPool.push_back(new RenderableNode());
		break;
	case Node_Type_Count:
	default:
		// assert
		break;
	}

	return m_nodesPool.back();
}

}
}

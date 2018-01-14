#include "ECSManager.h"
#include "core\EntityComponentSystem\Components\ComponentPool.h"

#include "Components\ModelComponent.h"
#include "Components\LayoutComponent.h"

#include "Entities\BaseEntity.h"

#include "Nodes\RenderableNode.h"

#include "Systems\RenderableSystem.h"

namespace aqua
{

ecs::ECSManager* patterns::Singleton<ecs::ECSManager>::s_instance = nullptr;

namespace ecs
{

bool ECSManager::Initialize()
{
	return false;
}

bool ECSManager::Shutdown()
{
	return false;
}

void ECSManager::LoadEntities()
{
	BaseComponent* comp = ComponentPool::GetInstance()->CreateComponent(Model_Component);
	m_entities.push_back(new BaseEntity());
	m_entities[0]->AddComponent(comp);

	RenderableNode* node = static_cast<RenderableNode*>(ComponentPool::GetInstance()->CreateNode(Renderable_Node));
	node->m_model = static_cast<ModelComponent*>(comp);

	comp = ComponentPool::GetInstance()->CreateComponent(Layout_Component);
	m_entities[0]->AddComponent(comp);
	node->m_layout = static_cast<LayoutComponent*>(comp);

	m_systems.push_back(new RenderableSystem());
	m_systems.back()->AddTarget(node);

}

void ECSManager::LoadSystems()
{
}

void ECSManager::Update()
{
	m_systems[0]->Update();
}

}
}
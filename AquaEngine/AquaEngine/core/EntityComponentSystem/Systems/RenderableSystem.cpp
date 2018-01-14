#include "RenderableSystem.h"
#include "core\EntityComponentSystem\Nodes\RenderableNode.h"
#include "core\EntityComponentSystem\Components\ModelComponent.h"
#include "core\EntityComponentSystem\Components\LayoutComponent.h"

#include "core\DirectX\DxResourcesManager.h"

namespace aqua
{
namespace ecs
{

void RenderableSystem::Update()
{
	RenderableNode* baseNodeIt = static_cast<RenderableNode*>(*m_targets.begin());
	
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext = DxResourcesManager::GetInstance()->GetD3DContext();
	unsigned int offset = 0;
	for (std::vector<BaseNode*>::size_type i = 0; i < m_targets.size(); i++)
	{
		d3dContext->IASetVertexBuffers(0, 1, (baseNodeIt + i)->m_model->m_vertexBuffer.GetAddressOf(), &ModelComponent::s_vertexTypestride, &offset);
		d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3dContext->IASetInputLayout((baseNodeIt + i)->m_layout->m_layout.Get());
		d3dContext->VSSetShader((baseNodeIt + i)->m_layout->m_vertexShader.Get(), NULL, 0);
		d3dContext->PSSetShader((baseNodeIt + i)->m_layout->m_pixelShader.Get(), NULL, 0);

		d3dContext->Draw(3, 0);
	}
}

}
}
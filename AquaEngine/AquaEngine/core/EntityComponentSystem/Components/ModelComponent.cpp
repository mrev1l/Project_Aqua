#include "ModelComponent.h"
#include "core\DirectX\DxResourcesManager.h"

#include <vector>

namespace aqua
{
namespace ecs
{

const unsigned int ModelComponent::s_vertexTypestride = sizeof(VertexType);

bool ModelComponent::InitializeComponent()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT result;

	std::vector<VertexType> vertices(3);

	vertices[0].m_pos = DirectX::XMFLOAT4( 0.0f,  0.0f, 0.0f, 1.0f); vertices[0].m_color = DirectX::XMFLOAT4(1.0f, .0f, .0f, 1.0f);
	vertices[1].m_pos = DirectX::XMFLOAT4( 0.0f,  1.0f, 0.0f, 1.0f); vertices[1].m_color = DirectX::XMFLOAT4(.0f, 1.0f, .0f, 1.0f);
	vertices[2].m_pos = DirectX::XMFLOAT4( 1.0f,  0.0f, 0.0f, 1.0f); vertices[2].m_color = DirectX::XMFLOAT4(.0f, .0f, 1.0f, 1.0f);

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = DxResourcesManager::GetInstance()->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	return result == S_OK;
}

}
}

#pragma once
#include "BaseComponent.h"
#include "core\Helpers\Dx.h"

namespace aqua
{
namespace ecs
{

class ModelComponent : public BaseComponent
{
public:
	bool InitializeComponent() override;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

	static const unsigned int s_vertexTypestride;

private:
	struct VertexType
	{
		DirectX::XMFLOAT4 m_pos;
		DirectX::XMFLOAT4 m_color;
	};
};

}
}
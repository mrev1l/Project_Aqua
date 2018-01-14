#pragma once
#include "BaseComponent.h"
#include "core\Helpers\Dx.h"

namespace aqua
{
namespace ecs
{

class LayoutComponent : public BaseComponent
{
public:
	bool InitializeComponent() override;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_layout;
};

}
}

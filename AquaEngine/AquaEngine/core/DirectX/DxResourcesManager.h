#pragma once
#include "core\Helpers\Dx.h"
#include "core\Patterns\Singleton.h"

namespace aqua
{

class DxResourcesManager : public patterns::Singleton<DxResourcesManager>
{
public:
	bool Initialize(const int _screenW, const int _screenH, const bool _vsync, HWND _hwnd,
		const bool _fullscreen, const float _screenDepth, const float _screenNear);
	bool Shutdown();

	void BeginScene(float* _color);
	void EndScene();

	Microsoft::WRL::ComPtr<ID3D11Device> GetD3DDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetD3DContext();

private:
	bool m_vsyncEnabled;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>			m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_deviceContext;
	Microsoft::WRL::ComPtr<ID2D1Factory1>			m_d2dFactory;
	Microsoft::WRL::ComPtr<ID2D1Device>				m_d2dDevice;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext>		m_d2dContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthDisabledStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_rasterState;
	//CAligned<D3DMatrices, 16> mProjectionWorldOrtho;

	void CreateDeviceIndependentResources();
	void CreateDeviceDependentResources(const unsigned int _screenW,
		const unsigned int _screenH, HWND _hwnd, const bool _fullscreen);
	void CreateTargetAndStates(const unsigned int _screenW,
		const unsigned int _screenH);
	void CreateDirect2DResources();

};

}
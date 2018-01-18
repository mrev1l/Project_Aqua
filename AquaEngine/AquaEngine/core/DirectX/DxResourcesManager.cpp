#include "StdAfx.h"
#include "DxResourcesManager.h"

namespace aqua
{

DxResourcesManager* patterns::Singleton<DxResourcesManager>::s_instance = nullptr;

bool DxResourcesManager::Initialize(const int _screenW, const int _screenH, const bool _vsync, HWND _hwnd, const bool _fullscreen, 
	const float _screenDepth, const float _screenNear)
{
	m_vsyncEnabled = true;

	CreateDeviceIndependentResources();
	CreateDeviceDependentResources(_screenW, _screenH, _hwnd, _fullscreen);
	CreateTargetAndStates(_screenW, _screenH);
	CreateDirect2DResources();

	return true;
}

bool DxResourcesManager::Shutdown()
{
	m_d2dContext.Reset();
	m_d2dDevice.Reset();

	m_depthDisabledStencilState.Reset();
	m_rasterState.Reset();
	m_depthStencilView.Reset();
	m_depthStencilState.Reset();
	m_depthStencilBuffer.Reset();
	m_renderTargetView.Reset();

	m_deviceContext.Reset();
	m_device.Reset();
	m_swapChain.Reset();

	m_d2dFactory.Reset();

	return true;
}

void DxResourcesManager::BeginScene(float * _color)
{
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), _color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DxResourcesManager::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsyncEnabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else 
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}
}

Microsoft::WRL::ComPtr<ID3D11Device> DxResourcesManager::GetD3DDevice()
{
	return m_device;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> DxResourcesManager::GetD3DContext()
{
	return m_deviceContext;
}

void DxResourcesManager::CreateDeviceIndependentResources()
{
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, IID_PPV_ARGS(m_d2dFactory.GetAddressOf()));
}

void DxResourcesManager::CreateDeviceDependentResources(const unsigned int _screenW, const unsigned int _screenH, HWND _hwnd, const bool _fullscreen)
{
	unsigned int numerator = 0;
	unsigned int denominator = 0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = _screenW;
	swapChainDesc.BufferDesc.Height = _screenH;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = _hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (_fullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, &swapChainDesc, m_swapChain.GetAddressOf(), m_device.GetAddressOf(), NULL, m_deviceContext.GetAddressOf());

	::vs_log(FAILED(result), ERROR_msg, "Call to D3D11CreateDeviceAndSwapChain has failed!");
}

void DxResourcesManager::CreateTargetAndStates(const unsigned int _screenW, const unsigned int _screenH)
{
	ID3D11Texture2D * backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC  depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;

	auto result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, m_renderTargetView.GetAddressOf());

	backBufferPtr->Release();
	backBufferPtr = 0;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = _screenW;
	depthBufferDesc.Height = _screenH;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, m_depthStencilBuffer.GetAddressOf());

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf());

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterDesc, m_rasterState.GetAddressOf());

	m_deviceContext->RSSetState(m_rasterState.Get());

	viewport.Width = (float)_screenW;
	viewport.Height = (float)_screenH;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_deviceContext->RSSetViewports(1, &viewport);

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthDisabledStencilState);
}

void DxResourcesManager::CreateDirect2DResources()
{
	D2D1_CREATION_PROPERTIES creationProps;
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDev;

	HRESULT result = m_device.As(&dxgiDev);

#ifdef _DEBUG
	creationProps.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	creationProps.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	creationProps.options = D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS;
	creationProps.threadingMode = D2D1_THREADING_MODE_MULTI_THREADED;

	result = D2D1CreateDevice(dxgiDev.Get(), creationProps, m_d2dDevice.GetAddressOf());

	result = m_d2dDevice->CreateDeviceContext(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		m_d2dContext.GetAddressOf());

	//Set up Direct2D render target bitmap, linking it to the swapchain
	Microsoft::WRL::ComPtr<IDXGISurface> dxgiBackBuffer;
	D2D1_PIXEL_FORMAT pixelFormat = { DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED };
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = { pixelFormat, 96.0f, 96.0f,
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, 0 };
	//Direct2D needs the dxgi version of the backbuffer surface pointer
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBitmap;

	result = m_d2dContext->CreateBitmapFromDxgiSurface(
		dxgiBackBuffer.Get(),
		&bitmapProperties,
		targetBitmap.GetAddressOf()
	);
	m_d2dContext->SetTarget(targetBitmap.Get());
}

}

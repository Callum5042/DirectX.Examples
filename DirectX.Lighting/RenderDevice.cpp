
#include "RenderDevice.h"
#include <SDL_messagebox.h>
#include <SDL_syswm.h>

#include <DirectXMath.h>
using namespace DirectX;

ID3D11Texture2D* g_pDepthStencil = nullptr;
ID3D11DepthStencilView* g_pDepthStencilView = nullptr;

struct SimpleVertex
{
	SimpleVertex(float x, float y, float z, XMFLOAT3 Normal) : x(x), y(y), z(z), Normal(Normal) {}

	float x;
	float y;
	float z;

	XMFLOAT3 Normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
};

ID3D11Buffer* g_pConstantBuffer = nullptr;
DirectX::XMMATRIX g_World;
DirectX::XMMATRIX g_View;
DirectX::XMMATRIX g_Projection;

RenderDevice::RenderDevice(MainWindow* mainWindow) : m_Window(mainWindow)
{

}

bool RenderDevice::Initialise()
{
	if (!CreateDevice())
		return false;

	if (!CreateSwapChain())
		return false;

	if (!CreateRenderTargetView())
		return false;

	// Setup the viewport
	SetViewport();

	// Raster
	SetRasterState();

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800;
	descDepth.Height = 600;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = m_Device->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, g_pDepthStencilView);

	// Create vertex buffer
	/*SimpleVertex vertices[] =
	{
		SimpleVertex(-0.5f, 0.5f, 0.0f, XMFLOAT3(0.0f, 1.0f, 0.0f)),
		SimpleVertex(0.5f, 0.5f, 0.0f, XMFLOAT3(0.0f, 1.0f, 0.0f)),
		SimpleVertex(0.5f, -0.5f, 0.0f, XMFLOAT3(0.0f, 1.0f, 0.0f)),
		SimpleVertex(-0.5f, -0.5f, 0.0f, XMFLOAT3(0.0f, 1.0f, 0.0f)),
	};*/

	/*WORD indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};*/

	SimpleVertex vertices[] =
	{
		{ -1.0f, 1.0f, -1.0f, XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ 1.0f, 1.0f, -1.0f, XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ 1.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ -1.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ -1.0f, -1.0f, -1.0f, XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ 1.0f, -1.0f, -1.0f, XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ 1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ -1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ -1.0f, -1.0f, 1.0f, XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ -1.0f, -1.0f, -1.0f, XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ -1.0f, 1.0f, -1.0f, XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ -1.0f, 1.0f, 1.0f, XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ 1.0f, -1.0f, 1.0f, XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ 1.0f, -1.0f, -1.0f, XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ 1.0f, 1.0f, -1.0f, XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ 1.0f, 1.0f, 1.0f, XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ -1.0f, -1.0f, -1.0f, XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ 1.0f, -1.0f, -1.0f, XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ 1.0f, 1.0f, -1.0f, XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ -1.0f, 1.0f, -1.0f, XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ -1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ 1.0f, -1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ 1.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ -1.0f, 1.0f, 1.0f, XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	// Vertex duffer description
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(vertices);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vInitData = {};
	vInitData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer = nullptr; 
	hr = m_Device->CreateBuffer(&vbd, &vInitData, &vertexBuffer);
	if (FAILED(hr))
		return false;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Index buffer description
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(indices);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iInitData = {};
	iInitData.pSysMem = indices;

	ID3D11Buffer* indexBuffer = nullptr;
	hr = m_Device->CreateBuffer(&ibd, &iInitData, &indexBuffer);
	if (FAILED(hr))
		return false;

	// Set vertex buffer
	m_DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = m_Device->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
	if (FAILED(hr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "CreateBuffer failed", nullptr);
		return false;
	}

	// Perspective View
	g_World = XMMatrixIdentity();
	
	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(eye, at, up);

	// Create the projection matrix for 3D rendering.
	float fieldOfView = 85 * DirectX::XM_PI / 180;
	float screenAspect = (float)800 / (float)600;
	g_Projection = XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, screenAspect, 0.01f, 100.0f);

	return true;
}

void RenderDevice::SetRasterState()
{
	D3D11_RASTERIZER_DESC rasterizerState;
	ZeroMemory(&rasterizerState, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerState.AntialiasedLineEnable = false;
	rasterizerState.CullMode = D3D11_CULL_NONE; // D3D11_CULL_FRONT or D3D11_CULL_NONE D3D11_CULL_BACK
	rasterizerState.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_SOLID  D3D11_FILL_WIREFRAME
	rasterizerState.DepthBias = 0;
	rasterizerState.DepthBiasClamp = 0.0f;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.FrontCounterClockwise = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.ScissorEnable = false;
	rasterizerState.SlopeScaledDepthBias = 0.0f;

	ID3D11RasterizerState* m_pRasterState;
	HRESULT result = m_Device->CreateRasterizerState(&rasterizerState, &m_pRasterState);
	m_DeviceContext->RSSetState(m_pRasterState);
}

void RenderDevice::SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)800;
	vp.Height = (FLOAT)600;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);
}

bool RenderDevice::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&m_Device,
		&featureLevel,
		&m_DeviceContext);

	if (FAILED(hr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "D3D11CreateDeviceAndSwapChain failed", nullptr);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Invalid Direct3D version failed", nullptr);
		return false;
	}

	return true;
}

bool RenderDevice::CreateSwapChain()
{
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(m_Window->GetWindow(), &wmInfo);
	HWND hwnd = wmInfo.info.win.window;

	// Description
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// Create swapchain
	IDXGIDevice* dxgiDevice = nullptr;
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	HRESULT hr = dxgiFactory->CreateSwapChain(m_Device, &sd, &m_SwapChain);
	if (FAILED(hr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Swapchain", nullptr);
		return false;
	}

	dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES);

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	return true;
}

bool RenderDevice::CreateRenderTargetView()
{
	ID3D11Resource* backBuffer = nullptr;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
	if (backBuffer == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "IDXGISwapChain::GetBuffer failed", nullptr);
		return false;
	}

	// Create render target
	HRESULT hr = m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
	if (FAILED(hr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "CreateRenderTargetView failed", nullptr);
		return false;
	}

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
	return true;
}

void RenderDevice::Render()
{
	// Clear window to blue
	static const float Blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&Blue));
	m_DeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// hing
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	// Animate the cube
	g_World = XMMatrixRotationY(t);

	// Setup our lighting parameters
    XMFLOAT4 vLightDirs[2] =
    {
        XMFLOAT4( -0.577f, 0.577f, -0.577f, 1.0f ),
        XMFLOAT4( 0.0f, 0.0f, -1.0f, 1.0f ),
    };

    XMFLOAT4 vLightColors[2] =
    {
        XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f ),
        XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f )
    };

	// Shader thing

	/*XMMATRIX mRotate = XMMatrixRotationY(-2.0f * t);
	XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[1]);
	vLightDir = XMVector3Transform(vLightDir, mRotate);
	XMStoreFloat4(&vLightDirs[1], vLightDir);*/

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	cb.vLightDir[0] = vLightDirs[0];
	cb.vLightColor[0] = vLightColors[0];
	cb.vLightDir[1] = vLightDirs[1];
	cb.vLightColor[1] = vLightColors[1];

	m_DeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	m_DeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	m_DeviceContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Render triangle
	m_DeviceContext->DrawIndexed(36, 0, 0);

	// Update window
	m_SwapChain->Present(0, 0);
}
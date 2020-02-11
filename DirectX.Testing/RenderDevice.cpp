
#include "RenderDevice.h"
#include <SDL_syswm.h>
#include <iostream>
#include <SDL_messagebox.h>
#include <fstream>

#include <DirectXMath.h>
using namespace DirectX;

#include <DDSTextureLoader.h>

#include "GameObject.h"

ID3D11Buffer* g_pConstantBuffer = nullptr;
DirectX::XMMATRIX g_World;
DirectX::XMMATRIX g_World2;
DirectX::XMMATRIX g_View;
DirectX::XMMATRIX g_Projection;

void Error(std::string&& err)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", err.c_str(), nullptr);
}

struct TextureVertex
{
	TextureVertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {}

	float x;
	float y;
	float z;

	float u;
	float v;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

RenderDevice::RenderDevice(MainWindow* window) : m_Window(window)
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
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)800;
	vp.Height = (FLOAT)600;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);

	// Raster
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

	// Create vertex shader
	if (!CreateVertexShader())
		return false;

	// Create pixel shader
	if (!CreatePixelShader())
		return false;

	TextureVertex vertices[] =
	{
		TextureVertex(-1.0f, 1.0f, 0.5f, 0.0f, 0.0f),
		TextureVertex(1.0f, 1.0f, 0.5f, 1.0f, 0.0f),
		TextureVertex(1.0f, -1.0f, 0.5f, 1.0f, 1.0f),
		TextureVertex(-1.0f, -1.0f, 0.5f, 0.0f, 1.0f),
	};

	WORD indices[] =
	{
		0, 1, 2,
		0, 2, 3
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
	HRESULT hr = m_Device->CreateBuffer(&vbd, &vInitData, &vertexBuffer);
	if (FAILED(hr))
		return false;

	// Set vertex buffer
	UINT stride = sizeof(TextureVertex);
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

	// Create texture
	hr = CreateDDSTextureFromFile(m_Device, L"textures/castle_wall/castle_wall_slates_diff_1k.dds", nullptr, &m_Texture);
	if (FAILED(hr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "CreateDDSTextureFromFile failed", nullptr);
		return false;
	}
	
	// Position
	g_World = XMMatrixScaling(50.0f , 50.0f, 1.0f);
	g_World *= XMMatrixTranslation(400.0f, 300.0f, 0.0f);

	g_World2 = XMMatrixScaling(50.0f , 50.0f, 1.0f);
	g_World2 *= XMMatrixTranslation(50.0f, 50.0f, 0.0f);

	// Initialize the view matrix
	XMVECTOR eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(eye, at, up);

	// Initialize the projection matrix
	g_Projection = XMMatrixOrthographicOffCenterLH(0.0f, 800.0f, 600.0f, 0.0f, 1.0f, 100.0f);

	return true;
}

void RenderDevice::Render()
{
	static const float Blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&Blue));

	// Set pipeline
	m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
	m_DeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);
	m_DeviceContext->PSSetShaderResources(0, 1, &m_Texture);

	// Renddeeeer
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	m_DeviceContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	m_DeviceContext->DrawIndexed(6, 0, 0);

	// Renddeeeer
	cb.mWorld = XMMatrixTranspose(g_World2);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	m_DeviceContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	m_DeviceContext->DrawIndexed(6, 0, 0);

	// Update window
	m_SwapChain->Present(0, 0);
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
	SDL_GetWindowWMInfo(m_Window->m_Window, &wmInfo);
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

bool RenderDevice::CreateVertexShader()
{
	std::ifstream vertexFile("VertexShader.cso", std::fstream::in | std::fstream::binary);
	if (!vertexFile.is_open())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Could not read VertexShader.cso", nullptr);
		return false;
	}

	vertexFile.seekg(0, vertexFile.end);
	int vertexsize = (int)vertexFile.tellg();
	vertexFile.seekg(0, vertexFile.beg);

	char* vertexbuffer = new char[vertexsize];
	vertexFile.read(vertexbuffer, vertexsize);

	HRESULT hr = m_Device->CreateVertexShader(
		vertexbuffer,
		vertexsize,
		nullptr,
		&m_VertexShader);

	if (FAILED(hr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "CreateVertexShader failed", nullptr);
		return false;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* vertexLayout = nullptr;
	m_Device->CreateInputLayout(layout, numElements, vertexbuffer, vertexsize, &vertexLayout);

	m_DeviceContext->IASetInputLayout(vertexLayout);

	return true;
}

bool RenderDevice::CreatePixelShader()
{
	std::ifstream pixelFile("PixelShader.cso", std::fstream::in | std::fstream::binary);
	if (!pixelFile.is_open())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Could not read PixelShader.cso", nullptr);
		return false;
	}

	pixelFile.seekg(0, pixelFile.end);
	int pixelsize = (int)pixelFile.tellg();
	pixelFile.seekg(0, pixelFile.beg);

	char* pixelbuffer = new char[pixelsize];
	pixelFile.read(pixelbuffer, pixelsize);

	HRESULT hr = m_Device->CreatePixelShader(
		pixelbuffer,
		pixelsize,
		nullptr,
		&m_PixelShader);

	if (FAILED(hr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "CreatePixelShader failed", nullptr);
		return false;
	}

	return true;
}

void RenderDevice::OnKeyDown(SDL_KeyboardEvent e)
{
	if (e.keysym.scancode == SDL_SCANCODE_RIGHT)
	{  
		g_World = g_World * XMMatrixTranslation(5.0f, 0.0f, 0.0f);
	}
	else if (e.keysym.scancode == SDL_SCANCODE_LEFT)
	{
		g_World = g_World * XMMatrixTranslation(-5.0f, 0.0f, 0.0f);
	}

	if (e.keysym.scancode == SDL_SCANCODE_UP)
	{
		g_World = g_World * XMMatrixTranslation(0.0f, -5.0f, 0.0f);
	}
	else if (e.keysym.scancode == SDL_SCANCODE_DOWN)
	{
		g_World = g_World * XMMatrixTranslation(0.0f, 5.0f, 0.0f);
	}
}

void RenderDevice::OnMouseMove(SDL_MouseMotionEvent e)
{
	/*g_World = XMMatrixIdentity();
	g_World = g_World * XMMatrixScaling(50.0f, 50.0f, 1.0f);
	g_World = g_World * XMMatrixTranslation(e.x, e.y, 0.0f);*/
}
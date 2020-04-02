
#include "Application.h"
#include <SDL_syswm.h>
#include <fstream>
#include <SDL_messagebox.h>
#include <DirectXMath.h>
using namespace DirectX;

struct SimpleVertex
{
	SimpleVertex(float x, float y, float z, float nx, float ny, float nz) : x(x), y(y), z(z)
	{
		normal = XMFLOAT3(nx, ny, nz);
	}

	float x;
	float y;
	float z;

	XMFLOAT3 normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

bool Application::OnInitialise()
{
	if (!CreateDevice())
		return false;

	if (!CreateSwapChain())
		return false;

	if (!CreateRenderTargetView())
		return false;

	SetViewport();

	// Create shaders
	if (!CreateVertexShader("D:\\Sources\\Testing\\DirectX.Testing\\bin\\DirectX.Lighting\\Debug-x64\\VertexShader.cso"))
		return false;

	if (!CreatePixelShader("D:\\Sources\\Testing\\DirectX.Testing\\bin\\DirectX.Lighting\\Debug-x64\\PixelShader.cso"))
		return false;

	// Drawing
	SimpleVertex vertices[] =
	{
		// Top
		{ -1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
		{ +1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
		{ +1.0f, +1.0f, +1.0f, 0.0f, 1.0f, 0.0f },
		{ -1.0f, +1.0f, +1.0f, 0.0f, 1.0f, 0.0f },

		// Bottom
		{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
		{ +1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
		{ +1.0f, -1.0f, +1.0f, 0.0f, -1.0f, 0.0f },
		{ -1.0f, -1.0f, +1.0f, 0.0f, -1.0f, 0.0f },

		// Left
		{ -1.0f, -1.0f, +1.0f, -1.0f, 0.0f, 0.0f },
		{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
		{ -1.0f, +1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
		{ -1.0f, +1.0f, +1.0f, -1.0f, 0.0f, 0.0f },

		// Right
		{ +1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f },
		{ +1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
		{ +1.0f, +1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
		{ +1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 0.0f },

		// Front
		{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, -1.0f },
		{ +1.0f, -1.0f, -1.0f, -1.0f, 0.0f, -1.0f },
		{ +1.0f, +1.0f, -1.0f, -1.0f, 0.0f, -1.0f },
		{ -1.0f, +1.0f, -1.0f, -1.0f, 0.0f, -1.0f },

		// Back
		{ -1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 1.0f },
		{ +1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 1.0f },
		{ +1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f },
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
	HRESULT hr = m_Device->CreateBuffer(&vbd, &vInitData, &vertexBuffer);
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
	hr = m_Device->CreateBuffer(&bd, nullptr, &m_ConstantBuffer);
	if (FAILED(hr))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "CreateBuffer failed", nullptr);
		return false;
	}

	// Perspective View
	m_World = XMMatrixIdentity();

	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = XMMatrixLookAtLH(eye, at, up);

	// Create the projection matrix for 3D rendering.
	float fieldOfView = 85 * DirectX::XM_PI / 180;
	float screenAspect = (float)800 / (float)600;
	m_Projection = XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, screenAspect, 0.01f, 100.0f);

	return true;
}

void Application::OnUpdate()
{
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	m_World = XMMatrixRotationY(t);
	// m_World *= XMMatrixTranslation(-0.0f, -0.0f, 1.0f);
}

void Application::OnRender()
{
	// Clear to blue
	static const float Blue[] = { 0.3f, 0.5f, 0.7f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&Blue));
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Bind shaders
	m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);

	// Shader thing
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(m_World);
	cb.mView = XMMatrixTranspose(m_View);
	cb.mProjection = XMMatrixTranspose(m_Projection);

	m_DeviceContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_DeviceContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	m_DeviceContext->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Render triangle
	m_DeviceContext->DrawIndexed(36, 0, 0);

	// Refresh screen
	m_SwapChain->Present(0, 0);
}

void Application::OnQuit()
{
	Exit();
}

void Application::OnResize(int width, int height)
{
	m_RenderTargetView->Release();
	m_DepthStencilView->Release();

	DX::ThrowIfFailed(m_SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));	
	CreateRenderTargetView();
	SetViewport();
}

bool Application::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	DX::ThrowIfFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &m_Device, &featureLevel, &m_DeviceContext));

	if (featureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		return false;
	}

	return true;
}

bool Application::CreateSwapChain()
{
	IDXGIFactory1* dxgiFactory1 = GetDXGIFactory();
	IDXGIFactory2* dxgiFactory2 = nullptr;
	DX::ThrowIfFailed(dxgiFactory1->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2)));
	if (dxgiFactory2 != nullptr)
	{
		ID3D11Device1* device1 = nullptr;
		DX::ThrowIfFailed(m_Device->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&device1)));

		ID3D11DeviceContext1* deviceContext1 = nullptr;
		(void)m_DeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&deviceContext1));

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = GetWindow()->GetWidth();
		sd.Height = GetWindow()->GetHeight();
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		IDXGISwapChain1* swapChain1 = nullptr;
		DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(m_Device, GetHwnd(), &sd, nullptr, nullptr, &swapChain1));

		DX::ThrowIfFailed(swapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_SwapChain)));

		dxgiFactory2->Release();
	}
	else
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = GetWindow()->GetWidth();
		sd.BufferDesc.Height = GetWindow()->GetHeight();
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = GetHwnd();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		DX::ThrowIfFailed(dxgiFactory1->CreateSwapChain(m_Device, &sd, &m_SwapChain));
	}

	dxgiFactory1->MakeWindowAssociation(GetHwnd(), DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory1->Release();
	return true;
}

bool Application::CreateRenderTargetView()
{
	// Render target view
	ID3D11Resource* backBuffer = nullptr;
	DX::ThrowIfFailed(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer)));
	if (backBuffer == nullptr)
	{
		return false;
	}

	DX::ThrowIfFailed(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));
	backBuffer->Release();

	// Depth stencil view
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = GetWindow()->GetWidth();
	descDepth.Height = GetWindow()->GetHeight();
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	DX::ThrowIfFailed(m_Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencil));
	if (m_DepthStencil == nullptr)
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	DX::ThrowIfFailed(m_Device->CreateDepthStencilView(m_DepthStencil, &descDSV, &m_DepthStencilView));

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	return true;
}

void Application::SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(GetWindow()->GetWidth());
	vp.Height = static_cast<FLOAT>(GetWindow()->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_DeviceContext->RSSetViewports(1, &vp);
}

bool Application::CreateVertexShader(std::string&& vertexShaderPath)
{
	std::ifstream vertexFile(vertexShaderPath, std::fstream::in | std::fstream::binary);
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

	DX::ThrowIfFailed(m_Device->CreateVertexShader(vertexbuffer, vertexsize, nullptr, &m_VertexShader));

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* vertexLayout = nullptr;
	m_Device->CreateInputLayout(layout, numElements, vertexbuffer, vertexsize, &vertexLayout);
	m_DeviceContext->IASetInputLayout(vertexLayout);

	delete[] vertexbuffer;
	return true;
}

bool Application::CreatePixelShader(std::string&& pixelShaderPath)
{
	std::ifstream pixelFile(pixelShaderPath, std::fstream::in | std::fstream::binary);
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

	DX::ThrowIfFailed(m_Device->CreatePixelShader(pixelbuffer, pixelsize, nullptr, &m_PixelShader));

	delete[] pixelbuffer;
	return true;
}

IDXGIFactory1* Application::GetDXGIFactory()
{
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		DX::ThrowIfFailed(m_Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)));

		IDXGIAdapter* adapter = nullptr;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(&adapter));

		DX::ThrowIfFailed(adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory)));

		adapter->Release();
		dxgiDevice->Release();
	}

	return dxgiFactory;
}

HWND Application::GetHwnd() const
{
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);

	SDL_Window* window = reinterpret_cast<SDL_Window*>(GetWindow()->GetWindow());
	SDL_GetWindowWMInfo(window, &wmInfo);
	return wmInfo.info.win.window;
}
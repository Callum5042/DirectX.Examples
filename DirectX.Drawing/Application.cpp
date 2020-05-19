
#include "Application.h"
#include <SDL_syswm.h>
#include <fstream>
#include <SDL_messagebox.h>

struct SimpleVertex
{
	SimpleVertex(float x, float y, float z) : x(x), y(y), z(z) {}

	float x;
	float y;
	float z;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

bool DX::Application::OnInitialise()
{
	m_Renderer = new Renderer();
	if (!m_Renderer->Initialise())
		return false;

	// Create shaders
	if (!CreateVertexShader("D:\\Sources\\Testing\\DirectX.Testing\\bin\\DirectX.Drawing\\Debug-x64\\VertexShader.cso"))
		return false;

	if (!CreatePixelShader("D:\\Sources\\Testing\\DirectX.Testing\\bin\\DirectX.Drawing\\Debug-x64\\PixelShader.cso"))
		return false;

	// Drawing
	SimpleVertex vertices[] =
	{
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f },

		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ -1.0f, 1.0f, 1.0f },

		{ 1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },

		{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
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
	HRESULT hr = m_Renderer->Device()->CreateBuffer(&vbd, &vInitData, &vertexBuffer);
	if (FAILED(hr))
		return false;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	m_Renderer->DeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Index buffer description
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(indices);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iInitData = {};
	iInitData.pSysMem = indices;

	ID3D11Buffer* indexBuffer = nullptr;
	hr = m_Renderer->Device()->CreateBuffer(&ibd, &iInitData, &indexBuffer);
	if (FAILED(hr))
		return false;

	// Set vertex buffer
	m_Renderer->DeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	m_Renderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = m_Renderer->Device()->CreateBuffer(&bd, nullptr, &m_ConstantBuffer);
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

void DX::Application::OnUpdate()
{
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	m_World = XMMatrixRotationY(t);
}

void DX::Application::OnRender()
{
	m_Renderer->Clear();

	// Bind shaders
	m_Renderer->DeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
	m_Renderer->DeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);

	// Shader thing
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(m_World);
	cb.mView = XMMatrixTranspose(m_View);
	cb.mProjection = XMMatrixTranspose(m_Projection);

	m_Renderer->DeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_Renderer->DeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	m_Renderer->DeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Render triangle
	m_Renderer->DeviceContext()->DrawIndexed(36, 0, 0);

	// Refresh screen
	m_Renderer->Draw();
}

void DX::Application::OnQuit()
{
	Exit();
}

void DX::Application::OnResize(int width, int height)
{
	m_Renderer->Resize(width, height);

	float fieldOfView = 85 * DirectX::XM_PI / 180;
	float screenAspect = (float)width / (float)height;
	m_Projection = XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, screenAspect, 0.01f, 100.0f);
}

bool DX::Application::CreateVertexShader(std::string&& vertexShaderPath)
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

	DX::ThrowIfFailed(m_Renderer->Device()->CreateVertexShader(vertexbuffer, vertexsize, nullptr, &m_VertexShader));

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* vertexLayout = nullptr;
	m_Renderer->Device()->CreateInputLayout(layout, numElements, vertexbuffer, vertexsize, &vertexLayout);
	m_Renderer->DeviceContext()->IASetInputLayout(vertexLayout);

	delete[] vertexbuffer;
	return true;
}

bool DX::Application::CreatePixelShader(std::string&& pixelShaderPath)
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

	DX::ThrowIfFailed(m_Renderer->Device()->CreatePixelShader(pixelbuffer, pixelsize, nullptr, &m_PixelShader));

	delete[] pixelbuffer;
	return true;
}
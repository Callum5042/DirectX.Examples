#include "Model.h"
#include "Application.h"

#include <DirectXMath.h>
#include <DDSTextureLoader.h>

struct Vertex
{
	Vertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v) {}

	float x;
	float y;
	float z;

	float u;
	float v;
};

_declspec(align(16)) struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;

	DirectX::XMMATRIX mTexture;
	int mUseAlpha;
};

DX::Model::~Model()
{
	DX::Release(m_ConstantBuffer);
}

void DX::Model::Load()
{
	// Drawing
	Vertex vertices[] =
	{
		{ -1.0f, -1.0f, -1.0f, 0.0f, 1.0f },
		{ -1.0f, +1.0f, -1.0f, 0.0f, 0.0f },
		{ +1.0f, +1.0f, -1.0f, 1.0f, 0.0f },
		{ +1.0f, -1.0f, -1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, +1.0f, 1.0f, 1.0f },
		{ +1.0f, -1.0f, +1.0f, 0.0f, 1.0f },
		{ +1.0f, +1.0f, +1.0f, 0.0f, 0.0f },
		{ -1.0f, +1.0f, +1.0f, 1.0f, 0.0f },

		{ -1.0f, +1.0f, -1.0f, 0.0f, 1.0f },
		{ -1.0f, +1.0f, +1.0f, 0.0f, 0.0f },
		{ +1.0f, +1.0f, +1.0f, 1.0f, 0.0f },
		{ +1.0f, +1.0f, -1.0f, 1.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f },
		{ +1.0f, -1.0f, -1.0f, 0.0f, 1.0f },
		{ +1.0f, -1.0f, +1.0f, 0.0f, 0.0f },
		{ -1.0f, -1.0f, +1.0f, 1.0f, 0.0f },

		{ -1.0f, -1.0f, +1.0f, 0.0f, 1.0f },
		{ -1.0f, +1.0f, +1.0f, 0.0f, 0.0f },
		{ -1.0f, +1.0f, -1.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f },

		{ +1.0f, -1.0f, -1.0f, 0.0f, 1.0f },
		{ +1.0f, +1.0f, -1.0f, 0.0f, 0.0f },
		{ +1.0f, +1.0f, +1.0f, 1.0f, 0.0f },
		{ +1.0f, -1.0f, +1.0f, 1.0f, 1.0f }
	};

	UINT indices[] =
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23,
	};

	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();

	// Vertex duffer description
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(vertices);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vInitData = {};
	vInitData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer = nullptr;
	DX::ThrowIfFailed(renderer->Device()->CreateBuffer(&vbd, &vInitData, &vertexBuffer));

	// Set vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	renderer->DeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Index buffer description
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(indices);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iInitData = {};
	iInitData.pSysMem = indices;

	ID3D11Buffer* indexBuffer = nullptr;
	DX::ThrowIfFailed(renderer->Device()->CreateBuffer(&ibd, &iInitData, &indexBuffer));

	// Set vertex buffer
	renderer->DeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	renderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	DX::ThrowIfFailed(renderer->Device()->CreateBuffer(&bd, nullptr, &m_ConstantBuffer));

	// Perspective View
	m_World = DirectX::XMMatrixIdentity();

	

	m_TextureMatrix = DirectX::XMMatrixIdentity();
}

void DX::Model::Update()
{
	auto& timer = reinterpret_cast<Application*>(Application::Get())->GetTimer();

	// Rotate
	if (m_TextureAnimateRotate)
	{
		m_TextureMatrix *= DirectX::XMMatrixTranslation(-0.5f, -0.5f, 0.0f);
		m_TextureMatrix *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(20.0f * (float)timer.DeltaTime()));
		m_TextureMatrix *= DirectX::XMMatrixTranslation(0.5f, 0.5f, 0.0f);
	}

	if (m_AnimateTranslate) 
	{
		m_TextureMatrix *= DirectX::XMMatrixTranslation(0.0f, -0.5f * (float)timer.DeltaTime(), 0.0f);
	}
}

void DX::Model::Render()
{
	auto& camera = reinterpret_cast<Application*>(Application::Get())->Camera();

	ConstantBuffer cb;
	cb.mWorld = DirectX::XMMatrixTranspose(m_World);
	cb.mView = DirectX::XMMatrixTranspose(camera->GetView());
	cb.mProjection = DirectX::XMMatrixTranspose(camera->GetProjection());
	cb.mTexture = DirectX::XMMatrixTranspose(m_TextureMatrix);
	cb.mUseAlpha = m_UseAlpha;

	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();
	renderer->DeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	renderer->DeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	renderer->DeviceContext()->PSSetShaderResources(0, 1, &m_DiffuseMapSRV );
	renderer->DeviceContext()->PSSetShaderResources(1, 1, &m_OpacityMapSRV );
	renderer->DeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);

	renderer->DeviceContext()->DrawIndexed(36, 0, 0);
}

void DX::Model::LoadTexture(std::wstring&& texture_path)
{
	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();

	ID3D11Resource* texResource = nullptr;
	DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(renderer->Device(), texture_path.c_str(), &texResource, &m_DiffuseMapSRV));
	texResource->Release();
}

void DX::Model::LoadOpacityTexture(std::wstring&& texture_path)
{
	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();

	ID3D11Resource* texResource = nullptr;
	DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(renderer->Device(), L"D:\\Sources\\Testing\\DirectX.Testing\\DirectX.Texturing\\Textures\\Fence003_2K_Opacity.dds", &texResource, &m_OpacityMapSRV));
	DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(renderer->Device(), texture_path.c_str(), &texResource, &m_OpacityMapSRV));
	texResource->Release();

	m_UseAlpha = true;
}

void DX::Model::SetWorld(float x, float y, float z)
{
	m_World = DirectX::XMMatrixTranslation(x, y, z);
}
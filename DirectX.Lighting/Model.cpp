#include "Model.h"
#include "Application.h"
#include "GeometryGenerator.h"

#include <DirectXMath.h>
#include <DDSTextureLoader.h>

_declspec(align(16)) struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;

	DirectX::XMMATRIX mTexture;
	int mUseAlpha;
};

_declspec(align(16)) struct LightBuffer
{
	DirectX::XMFLOAT3 mCameraPos;
};

DX::Model::~Model()
{
	DX::Release(m_ConstantBuffer);
}

void DX::Model::Load()
{
	Geometry::CreateBox(1.0f, 1.0f, 1.0f, &meshdata);

	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();

	// Vertex duffer description
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Vertex) * static_cast<UINT>(meshdata.vertices.size());
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vInitData = {};
	vInitData.pSysMem = &meshdata.vertices[0];

	ID3D11Buffer* vertexBuffer = nullptr;
	DX::ThrowIfFailed(renderer->Device()->CreateBuffer(&vbd, &vInitData, &vertexBuffer));

	// Set vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	renderer->DeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Index buffer description
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * static_cast<UINT>(meshdata.indices.size());
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iInitData = {};
	iInitData.pSysMem = &meshdata.indices[0];

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

	// Light buffer
	D3D11_BUFFER_DESC lightBufferDesc = {};
	lightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = 0;
	DX::ThrowIfFailed(renderer->Device()->CreateBuffer(&lightBufferDesc, nullptr, &m_LightBuffer));

	// Perspective View
	m_World = DirectX::XMMatrixIdentity();

	// Texture matrix
	m_TextureMatrix = DirectX::XMMatrixIdentity();
}

void DX::Model::Update()
{
	auto& timer = reinterpret_cast<Application*>(Application::Get())->GetTimer();

	// static float angle = 0.0f;
	// angle += 1.0f * timer.DeltaTime();

	//m_World *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(20.0f * (float)timer.DeltaTime()));

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

	LightBuffer lb;
	lb.mCameraPos = camera->GetPosition();

	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();
	renderer->DeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	renderer->DeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	renderer->DeviceContext()->PSSetConstantBuffers(1, 1, &m_LightBuffer);


	renderer->DeviceContext()->PSSetShaderResources(0, 1, &m_DiffuseMapSRV );
	renderer->DeviceContext()->PSSetShaderResources(1, 1, &m_OpacityMapSRV );

	renderer->DeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);
	renderer->DeviceContext()->UpdateSubresource(m_LightBuffer, 0, nullptr, &lb, 0, 0);

	renderer->DeviceContext()->DrawIndexed(static_cast<UINT>(meshdata.indices.size()), 0, 0);
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
	//DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(renderer->Device(), L"D:\\Sources\\Testing\\DirectX.Testing\\DirectX.Texturing\\Textures\\Fence003_2K_Opacity.dds", &texResource, &m_OpacityMapSRV));
	DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(renderer->Device(), texture_path.c_str(), &texResource, &m_OpacityMapSRV));
	texResource->Release();

	m_UseAlpha = true;
}

void DX::Model::SetWorld(float x, float y, float z)
{
	m_World = DirectX::XMMatrixTranslation(x, y, z);
}
#include "Model.h"
#include <DirectXMath.h>
#include "Application.h"

struct Vertex
{
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

	float x;
	float y;
	float z;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
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

	UINT indices[] =
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
}

void DX::Model::Render()
{
	auto& camera = reinterpret_cast<Application*>(Application::Get())->Camera();

	ConstantBuffer cb;
	cb.mWorld = DirectX::XMMatrixTranspose(m_World);
	cb.mView = DirectX::XMMatrixTranspose(camera->GetView());
	cb.mProjection = DirectX::XMMatrixTranspose(camera->GetProjection());

	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();
	renderer->DeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	renderer->DeviceContext()->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	renderer->DeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);

	renderer->DeviceContext()->DrawIndexed(36, 0, 0);
}
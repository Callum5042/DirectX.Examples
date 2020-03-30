#include "Model.h"
#include "Engine.h"
#include "RenderDevice.h"
#include <SDL_messagebox.h>
#include <DirectXMath.h>
using namespace DirectX;

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	XMFLOAT4 vLightDir;
	XMFLOAT4 vLightColor;
	XMFLOAT4 vLightAmbient;
};

ID3D11Buffer* g_pConstantBuffer1 = nullptr;
DirectX::XMMATRIX g_World;
DirectX::XMMATRIX g_View;
DirectX::XMMATRIX g_Projection;

struct SimpleVertex
{
	SimpleVertex(float x, float y, float z, XMFLOAT3 Normal) : x(x), y(y), z(z), Normal(Normal) {}

	float x;
	float y;
	float z;

	XMFLOAT3 Normal;
};

bool Model::Init()
{
	auto device = Engine::GetInstance()->GetRenderDevice()->GetDevice();
	auto deviceContext = Engine::GetInstance()->GetRenderDevice()->GetDeviceContext();

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
	HRESULT hr = device->CreateBuffer(&vbd, &vInitData, &vertexBuffer);
	if (FAILED(hr))
		return false;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Index buffer description
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(indices);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iInitData = {};
	iInitData.pSysMem = indices;

	ID3D11Buffer* indexBuffer = nullptr;
	hr = device->CreateBuffer(&ibd, &iInitData, &indexBuffer);
	if (FAILED(hr))
		return false;

	// Set vertex buffer
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = device->CreateBuffer(&bd, nullptr, &g_pConstantBuffer1);
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

void Model::Update()
{
	// hing
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	//// Animate the cube
	g_World = XMMatrixRotationY(t);


	g_World *= XMMatrixTranslation(X, Y, Z);
}

void Model::Render()
{
	auto deviceContext = Engine::GetInstance()->GetRenderDevice()->GetDeviceContext();

	// Setup our lighting parameters
	//XMFLOAT4 lightDirection = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	//XMFLOAT4 lightColour = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	XMFLOAT4 lightDirection = XMFLOAT4(-0.0f, 0.0f, -0.5f, 1.0f); 
	XMFLOAT4 lightColour = XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f);
	XMFLOAT4 lightAmbient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	// Shader thing
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	cb.vLightDir = lightDirection;
	cb.vLightColor = lightColour;
	cb.vLightAmbient = lightAmbient;

	deviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer1);
	deviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer1);

	deviceContext->UpdateSubresource(g_pConstantBuffer1, 0, nullptr, &cb, 0, 0);

	// Render triangle
	deviceContext->DrawIndexed(36, 0, 0);
}

void Model::OnMouseDown(MouseData&& data)
{
	if (data.button == MouseButton::MOUSE_LMASK)
	{
		Z -= 1;
	}
	else if (data.button == MouseButton::MOUSE_RMASK)
	{
		Z += 1;
	}
}
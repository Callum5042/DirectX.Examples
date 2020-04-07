#pragma once

#include "Engine.h"
#include "WindowEvents.h"
#include "InputEvents.h"

#include <d3d11_4.h>
#include <DirectXMath.h>
using namespace DirectX;

class Application : public Engine, public Events::WindowListener, public Events::InputListener
{
public:
	Application() = default;

	bool OnInitialise() override;
	void OnUpdate() override;
	void OnRender() override;

	// Window Events
	void OnQuit() override;
	void OnResize(int width, int height) override;

	// Key Events
	void OnKeyDown(Events::KeyData&& data) override;

private:

	ID3D11Device* m_Device = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;
	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11Texture2D* m_DepthStencil = nullptr;
	ID3D11RenderTargetView* m_RenderTargetView = nullptr;
	ID3D11DepthStencilView* m_DepthStencilView = nullptr;
	ID3D11Buffer* m_ConstantBuffer = nullptr;

	UINT m_4xMsaaQuality;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Projection;

	ID3D11ShaderResourceView* m_DiffuseMapSRV;

	bool CreateDevice();
	bool CreateSwapChain();
	bool CreateRenderTargetView();
	void SetViewport();

	bool CreateVertexShader(std::string&& vertexShaderPath);
	bool CreatePixelShader(std::string&& pixelShaderPath);

	IDXGIFactory1* GetDXGIFactory();
	HWND GetHwnd() const;

	float m_PosX = 0.0f;
	float m_PosZ = 0.0f;
};

namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw std::exception();
		}
	}
}
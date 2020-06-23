#pragma once

#include <d3d11_4.h>
#include <exception>

namespace DX
{
	class Renderer
	{
	public:
		Renderer() = default;
		virtual ~Renderer();

		bool Initialise();

		void SetBlendState();

		void Clear();
		void Draw();

		void Resize(int width, int height);

		constexpr ID3D11Device* Device() { return m_Device; }
		constexpr ID3D11DeviceContext* DeviceContext() { return m_DeviceContext; }

		void SetWireframe(bool wireframe);

	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11Texture2D* m_DepthStencil = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;

		ID3D11RasterizerState* m_RasterStateSolid = nullptr;
		ID3D11RasterizerState* m_RasterStateWireframe = nullptr;

		ID3D11SamplerState* m_AnisotropicSampler = nullptr;

		UINT m_4xMsaaQuality = 0;


		bool CreateDevice();
		bool CreateSwapChain();
		bool CreateRenderTargetView();
		void SetViewport();

		void CreateRasterStateSolid();
		void CreateRasterStateWireframe();

		void CreateAnisotropicSampler();

		void CreateDepthStencil();

		IDXGIFactory1* GetDXGIFactory();
		HWND GetHwnd() const;
	};

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw std::exception();
		}
	}

	inline void Release(IUnknown* ref)
	{
		if (ref != nullptr)
		{
			ref->Release();
			ref = nullptr;
		}
	}
}
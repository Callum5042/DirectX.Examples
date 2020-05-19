#pragma once

#include <d3d11_4.h>
#include <exception>

namespace DX
{
	class Renderer
	{
	public:
		bool Initialise();

		void Clear();
		void Draw();

	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11Texture2D* m_DepthStencil = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;

		bool CreateDevice();
		bool CreateSwapChain();
		bool CreateRenderTargetView();
		void SetViewport();

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
}
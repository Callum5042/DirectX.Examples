#pragma once

#include "MainWindow.h"
#include <d3d11.h>

class RenderDevice
{
public:
	RenderDevice(MainWindow* window);

	bool Initialise();
	void SetRasterState();
	void Render();

	inline ID3D11Device* GetDevice() { return m_Device; }
	inline ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; }

private:
	MainWindow* m_Window = nullptr;

	ID3D11Device* m_Device = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;
	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11RenderTargetView* m_RenderTargetView = nullptr;

	bool CreateDevice();
	bool CreateSwapChain();
	bool CreateRenderTargetView();
	void SetViewport();
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
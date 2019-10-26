#pragma once

#include "MainWindow.h"
#include <d3d11.h>

class RenderDevice
{
public:
	RenderDevice(MainWindow* window);

	bool Initialise();

	void Render();

private:
	MainWindow* m_Window = nullptr;

	ID3D11Device* m_Device = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;
	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11RenderTargetView* m_RenderTargetView = nullptr;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;

	bool CreateDevice();
	bool CreateSwapChain();
	bool CreateRenderTargetView();

	bool CreateVertexShader();
	bool CreatePixelShader();
};
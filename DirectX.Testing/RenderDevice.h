#pragma once

#include "MainWindow.h"
#include <d3d11.h>
#include <SDL_events.h>

class RenderDevice
{
public:
	RenderDevice(MainWindow* window);

	bool Initialise();
	void Render();

	void OnKeyDown(SDL_KeyboardEvent e);
	void OnMouseMove(SDL_MouseMotionEvent e);

private:
	MainWindow* m_Window = nullptr;

	ID3D11Device* m_Device = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;
	IDXGISwapChain* m_SwapChain = nullptr;
	ID3D11RenderTargetView* m_RenderTargetView = nullptr;

	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;

	ID3D11ShaderResourceView* m_Texture = nullptr;
	ID3D11SamplerState* m_SamplerLinear = nullptr;

	bool CreateDevice();
	bool CreateSwapChain();
	bool CreateRenderTargetView();

	bool CreateVertexShader();
	bool CreatePixelShader();
};
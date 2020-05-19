#pragma once

#include "Engine.h"
#include "WindowEvents.h"
#include "Renderer.h"

#include <d3d11_4.h>
#include <DirectXMath.h>
using namespace DirectX;

namespace DX
{
	class Application : public Engine, public Events::WindowListener
	{
	public:
		Application() = default;

		bool OnInitialise() override;
		void OnUpdate() override;
		void OnRender() override;

		// Window Events
		void OnQuit() override;
		void OnResize(int width, int height) override;

	private:
		Renderer* m_Renderer = nullptr;

		ID3D11Buffer* m_ConstantBuffer = nullptr;
		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;

		XMMATRIX m_World;
		XMMATRIX m_View;
		XMMATRIX m_Projection;

		bool CreateVertexShader(std::string&& vertexShaderPath);
		bool CreatePixelShader(std::string&& pixelShaderPath);
	};
}
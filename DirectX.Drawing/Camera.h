#pragma once

#include <DirectXMath.h>

namespace DX
{
	class Camera
	{
	public:
		Camera() = default;

		void Update();
		void Resize(int width, int height);

		constexpr DirectX::XMMATRIX GetView() { return m_View; }
		constexpr DirectX::XMMATRIX GetProjection() { return m_Projection; }

	private:
		DirectX::XMMATRIX m_View;
		DirectX::XMMATRIX m_Projection;
		DirectX::XMFLOAT3 m_Position;
	};
}
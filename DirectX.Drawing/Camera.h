#pragma once

#include "InputEvents.h"
#include <DirectXMath.h>

namespace DX
{
	class Camera : public Events::InputListener
	{
	public:
		Camera();

		void Update();
		void Resize(int width, int height);

		constexpr DirectX::XMMATRIX GetView() { return m_View; }
		constexpr DirectX::XMMATRIX GetProjection() { return m_Projection; }

		// Mouse events
		void OnMouseMotion(MouseData&& data) override;

	private:
		DirectX::XMMATRIX m_View;
		DirectX::XMMATRIX m_Projection;
		DirectX::XMFLOAT3 m_Position;

		float m_FOV = 85.0;

		float m_Pitch = 30.0f;
		float m_Yaw = 0.0f;
	};
}
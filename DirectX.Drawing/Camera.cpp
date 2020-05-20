#include "Camera.h"
#include <SDL_mouse.h>

DX::Camera::Camera()
{
	m_Position = DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f);
}

void DX::Camera::Update()
{
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m_Position);
	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(m_Pitch), DirectX::XMConvertToRadians(m_Yaw), 0);
	position = XMVector3TransformCoord(position, camRotationMatrix);

	DirectX::XMVECTOR eye = position;
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = DirectX::XMMatrixLookAtLH(eye, at, up);
}

void DX::Camera::Resize(int width, int height)
{
	float fieldOfView = 85 * DirectX::XM_PI / 180;
	float screenAspect = (float)width / (float)height;
	m_Projection = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.01f, 100.0f);
}

void DX::Camera::OnMouseDown(MouseData&& data)
{
	if (data.button == MouseButton::MOUSE_LMASK)
	{
		m_LastMouseCoordinates.first = data.x;
		m_LastMouseCoordinates.second = data.y;
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}

void DX::Camera::OnMouseReleased(MouseData&& data)
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_WarpMouseGlobal(m_LastMouseCoordinates.first, m_LastMouseCoordinates.second);
}

void DX::Camera::OnMouseMotion(MouseData&& data)
{
	if (data.button == MouseButton::MOUSE_LMASK)
	{
		m_Yaw += (data.xrel * 0.25f);
		m_Pitch += (data.yrel * 0.25f);

		// Make sure it stays between 0-360
		if (m_Pitch > 360)
		{
			m_Pitch = m_Pitch - 360.0f;
		}
		else if (m_Pitch < 0)
		{
			m_Pitch = m_Pitch + 360.0f;
		}

		// Make sure it stays between 0-360
		if (m_Yaw > 360)
		{
			m_Yaw = m_Yaw - 360.0f;
		}
		else if (m_Yaw < 0)
		{
			m_Yaw = m_Yaw + 360.0f;
		}
	}
}
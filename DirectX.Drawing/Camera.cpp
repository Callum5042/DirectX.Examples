#include "Camera.h"

void DX::Camera::Update()
{
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
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
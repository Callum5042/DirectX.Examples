#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>

namespace DX
{
	class Model
	{
	public:
		Model() = default;
		virtual ~Model();

		void Load();
		void Render();

		void SetWorld(float x, float y, float z);

	private:
		ID3D11Buffer* m_ConstantBuffer = nullptr;

		DirectX::XMMATRIX m_World;
	};
}
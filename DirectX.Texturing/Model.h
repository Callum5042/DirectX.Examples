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

	private:
		DirectX::XMMATRIX m_World;
		ID3D11Buffer* m_ConstantBuffer = nullptr;
		ID3D11ShaderResourceView* m_DiffuseMapSRV = nullptr;
	};
}
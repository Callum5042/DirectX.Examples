#pragma once

#include <d3d11_4.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include <string>

namespace DX
{
	class Model
	{
	public:
		Model() = default;
		virtual ~Model();

		void Load();
		void Update();
		void Render();

		void LoadTexture(std::wstring&& texture_path);
		void LoadOpacityTexture(std::wstring&& texture_path);

		void SetWorld(float x, float y, float z);

	private:
		DirectX::XMMATRIX m_World;
		DirectX::XMMATRIX m_TextureMatrix;

		MeshData meshdata;

		ID3D11Buffer* m_ConstantBuffer = nullptr;
		ID3D11ShaderResourceView* m_DiffuseMapSRV = nullptr;
		ID3D11ShaderResourceView* m_OpacityMapSRV = nullptr;

		// Texture settings
		bool m_TextureAnimateRotate = false;
		bool m_AnimateTranslate = false;
		bool m_UseAlpha = false;
	};
}
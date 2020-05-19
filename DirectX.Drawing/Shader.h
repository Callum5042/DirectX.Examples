#pragma once

#include "Renderer.h"
#include <string>

namespace DX
{
	class Shader
	{
	public:
		Shader() = default;

		bool Load(std::string&& vertexShaderPath, std::string&& pixelShaderPath);
		void Use();

	private:
		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;

		bool CreateVertexShader(std::string&& vertexShaderPath);
		bool CreatePixelShader(std::string&& pixelShaderPath);

		void DefineInputLayout(char* vertexBuffer, int vertexBufferSize);
	};
}
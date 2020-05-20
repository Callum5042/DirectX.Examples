#include "Shader.h"
#include "Application.h"
#include <fstream>
#include <SDL_messagebox.h>

bool DX::Shader::Load(std::string&& vertexShaderPath, std::string&& pixelShaderPath)
{
	if (!CreateVertexShader(std::move(vertexShaderPath)))
		return false;

	if (!CreatePixelShader(std::move(pixelShaderPath)))
		return false;

	return true;
}

void DX::Shader::Use()
{
	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();

	renderer->DeviceContext()->VSSetShader(m_VertexShader, nullptr, 0);
	renderer->DeviceContext()->PSSetShader(m_PixelShader, nullptr, 0);
}

bool DX::Shader::CreateVertexShader(std::string&& vertexShaderPath)
{
	std::ifstream vertexFile(vertexShaderPath, std::fstream::in | std::fstream::binary);
	if (!vertexFile.is_open())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Could not read VertexShader.cso", nullptr);
		return false;
	}

	vertexFile.seekg(0, vertexFile.end);
	int vertexsize = (int)vertexFile.tellg();
	vertexFile.seekg(0, vertexFile.beg);

	char* vertexbuffer = new char[vertexsize];
	vertexFile.read(vertexbuffer, vertexsize);

	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();
	DX::ThrowIfFailed(renderer->Device()->CreateVertexShader(vertexbuffer, vertexsize, nullptr, &m_VertexShader));
	DefineInputLayout(vertexbuffer, vertexsize);

	delete[] vertexbuffer;
	return true;
}

bool DX::Shader::CreatePixelShader(std::string&& pixelShaderPath)
{
	std::ifstream pixelFile(pixelShaderPath, std::fstream::in | std::fstream::binary);
	if (!pixelFile.is_open())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Could not read PixelShader.cso", nullptr);
		return false;
	}

	pixelFile.seekg(0, pixelFile.end);
	int pixelsize = (int)pixelFile.tellg();
	pixelFile.seekg(0, pixelFile.beg);

	char* pixelbuffer = new char[pixelsize];
	pixelFile.read(pixelbuffer, pixelsize);

	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();
	DX::ThrowIfFailed(renderer->Device()->CreatePixelShader(pixelbuffer, pixelsize, nullptr, &m_PixelShader));

	delete[] pixelbuffer;
	return true;
}

void DX::Shader::DefineInputLayout(char* vertexBuffer, int vertexBufferSize)
{
	auto& renderer = reinterpret_cast<Application*>(Application::Get())->Renderer();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* vertexLayout = nullptr;
	renderer->Device()->CreateInputLayout(layout, numElements, vertexBuffer, vertexBufferSize, &vertexLayout);
	renderer->DeviceContext()->IASetInputLayout(vertexLayout);
}
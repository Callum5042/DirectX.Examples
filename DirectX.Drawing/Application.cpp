
#include "Application.h"
#include <SDL_syswm.h>
#include <SDL_messagebox.h>

struct SimpleVertex
{
	SimpleVertex(float x, float y, float z) : x(x), y(y), z(z) {}

	float x;
	float y;
	float z;
};

bool DX::Application::OnInitialise()
{
	m_Renderer = new DX::Renderer();
	if (!m_Renderer->Initialise())
		return false;

	// Create shaders
	m_Shader = new DX::Shader();
	if (!m_Shader->Load("D:\\Sources\\Testing\\DirectX.Testing\\bin\\DirectX.Drawing\\Debug-x64\\VertexShader.cso", "D:\\Sources\\Testing\\DirectX.Testing\\bin\\DirectX.Drawing\\Debug-x64\\PixelShader.cso"))
		return false;

	m_Model = new DX::Model();
	m_Model->Load();

	m_Camera = new DX::Camera();

	return true;
}

void DX::Application::OnUpdate()
{
	m_Camera->Update();
}

void DX::Application::OnRender()
{
	// Clear screen
	m_Renderer->Clear();

	// Bind shaders
	m_Shader->Use();

	// Render
	m_Model->Render();

	// Refresh screen
	m_Renderer->Draw();
}

void DX::Application::OnQuit()
{
	Exit();
}

void DX::Application::OnResize(int width, int height)
{
	m_Renderer->Resize(width, height);
	m_Camera->Resize(width, height);
}
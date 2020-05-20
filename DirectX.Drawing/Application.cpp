
#include "Application.h"

bool DX::Application::OnInitialise()
{
	if (!m_Renderer->Initialise())
		return false;

	if (!m_Shader->Load("D:\\Sources\\Testing\\DirectX.Testing\\bin\\DirectX.Drawing\\Debug-x64\\VertexShader.cso", "D:\\Sources\\Testing\\DirectX.Testing\\bin\\DirectX.Drawing\\Debug-x64\\PixelShader.cso"))
		return false;

	m_Model->Load();

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
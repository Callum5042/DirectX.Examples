#include "Application.h"

bool DX::Application::OnInitialise()
{
	if (!m_Renderer->Initialise())
		return false;

	if (!m_Shader->Load("VertexShader.cso", "PixelShader.cso"))
		return false;

	m_Model->Load();

	// Crate Texture
	m_Model->LoadTexture(L"Textures\\crate_diffuse_mipmap.dds");

	// Transparent cage
	//m_Model->LoadTexture(L"Textures\\Fence003_2K_Color.dds");
	//m_Model->LoadOpacityTexture(L"Textures\\Fence003_2K_Opacity.dds");

	// Translate Texture
	//m_Model->LoadTexture(L"Textures\\water_001_diffuse.dds");
	//m_Model->m_AnimateTranslate = true;

	// Rotate Texture
	//m_Model->LoadTexture(L"Textures\\fireball.dds");
	//m_Model->m_TextureAnimateRotate = true;

	return true;
}

void DX::Application::OnUpdate()
{
	m_Camera->Update();
	m_Model->Update();
}

void DX::Application::OnRender()
{
	// Clear screen
	m_Renderer->Clear();

	// Bind shaders
	m_Shader->Use();
	m_Renderer->SetWireframe(m_Wireframe);

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

void DX::Application::OnKeyDown(Events::KeyData&& data)
{
	if (data.key == SDL_SCANCODE_1)
	{
		m_Wireframe = !m_Wireframe;
	}
}
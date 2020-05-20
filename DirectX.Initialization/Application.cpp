
#include "Application.h"
#include <iostream>

DX::Application::~Application()
{
}

bool DX::Application::OnInitialise()
{
	if (!m_Renderer->Initialise())
		return false;

	return true;
}

void DX::Application::OnRender()
{
	m_Renderer->Clear();

	m_Renderer->Draw();
}

void DX::Application::OnResize(int width, int height)
{
	m_Renderer->Resize(width, height);
}

void DX::Application::OnQuit()
{
	Exit();
}
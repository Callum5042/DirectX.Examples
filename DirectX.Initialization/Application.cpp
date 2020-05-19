
#include "Application.h"
#include <iostream>

bool DX::Application::OnInitialise()
{
	m_Timer = new Timer();
	m_Timer->Start();

	m_Renderer = new Renderer();
	if (!m_Renderer->Initialise())
		return false;

	return true;
}

void DX::Application::OnUpdate()
{
	m_Timer->Tick();
	CalculateFrameStats();
}

void DX::Application::OnRender()
{
	m_Renderer->Clear();

	m_Renderer->Draw();
}

void DX::Application::OnQuit()
{
	Exit();
}

void DX::Application::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((m_Timer->TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::cout << "FPS: " << fps << '\n';
		// m_FPS = fps;

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
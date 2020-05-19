
#include "Engine.h"
#include <SDL.h>
#include <iostream>

Engine* Engine::m_Instance = nullptr;

Engine::Engine()
{
	m_Instance = this;
}

Engine::~Engine()
{
}

int Engine::Execute(int argc, char** argv)
{
	if (!Initialise())
		return -1;

	while (m_Running)
	{
		m_Timer.Tick();
		m_EventDispatcher->PollEvents();

		OnUpdate();
		OnRender();

		CalculateFrameStats();
	}

	return 0;
}

void Engine::Exit()
{
	m_Running = false;
}

bool Engine::OnInitialise()
{
	return false;
}

void Engine::OnUpdate()
{
}

bool Engine::Initialise()
{
	m_Timer.Start();

	// Create window
	m_MainWindow = std::unique_ptr<MainWindow>(new MainWindow());
	if (!m_MainWindow->Create("OpenGL", 800, 600))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		return false;
	}

	// Create event dispatcher
	m_EventDispatcher = std::unique_ptr<Events::EventDispatcher>(new Events::EventDispatcher());

	return OnInitialise();
}

void Engine::OnRender()
{

}

void Engine::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((m_Timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		m_FPS = fps;
		std::cout << m_FPS << '\n';

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
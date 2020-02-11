
#include "Engine.h"
#include <SDL.h>

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
		m_EventDispatcher->PollEvents();

		OnUpdate();
	}

	return 0;
}

Events::EventDispatcher* Engine::GetEventDispatcher() const
{
	return m_EventDispatcher;
}

void Engine::Exit()
{
	m_Running = false;
}

void Engine::OnUpdate()
{
}

bool Engine::Initialise()
{
	// Create window
	m_MainWindow = new MainWindow();
	if (!m_MainWindow->Create("DirectX", 800, 600))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		return false;
	}

	// Create event dispatcher
	m_EventDispatcher = new Events::EventDispatcher();

	return true;
}

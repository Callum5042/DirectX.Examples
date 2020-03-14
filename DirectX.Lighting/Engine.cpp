
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

		m_Shader->BindShaders();
		m_RenderDevice->Render();
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

bool Engine::OnInitialise()
{
	return false;
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

	// Create render device
	m_RenderDevice = new RenderDevice(m_MainWindow);
	if (!m_RenderDevice->Initialise())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "RenderDevice::Initialise failed!", nullptr);
		return false;
	}

	// Create shader
	m_Shader = new Shader(m_RenderDevice, m_MainWindow);

	// Create event dispatcher
	m_EventDispatcher = new Events::EventDispatcher();

	return OnInitialise();
}

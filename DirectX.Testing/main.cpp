#pragma warning (disable: 26812)
#pragma warning (disable: 6387)

#include <iostream>
#include <SDL.h>

#include "MainWindow.h"
#include "RenderDevice.h"

int main(int argc, char**)
{
	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL_Init failed", nullptr);
		return -1;
	}

	// Create window
	MainWindow window;
	if (!window.Create())
		return -1;

	// Rendering device
	RenderDevice renderDevice(&window);
	if (!renderDevice.Initialise())
		return -1;
	
	// Main loop
	SDL_Event e;
	bool running = true;
	while (running)
	{
		if (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
		else
		{
			renderDevice.Render();
		}
	}

	// Quit
	SDL_Quit();
	return 0;
}
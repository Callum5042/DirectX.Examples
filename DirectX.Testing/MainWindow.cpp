
#include "MainWindow.h"
#include <SDL_messagebox.h>

MainWindow::MainWindow()
{
}

bool MainWindow::Create()
{
	m_Window = SDL_CreateWindow("DirectX - Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	if (m_Window == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL_CreateWindow failed", nullptr);
		return false;
	}

	return true;
}

#include "MainWindow.h"

MainWindow::MainWindow() 
{
}

MainWindow::~MainWindow()
{
	Destroy();
}

bool MainWindow::Create(std::string&& title, int width, int height)
{
	m_WindowHandle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (m_WindowHandle == nullptr)
	{
		return false;
	}

	return true;
}

void MainWindow::Destroy()
{
	SDL_DestroyWindow(m_WindowHandle);
}

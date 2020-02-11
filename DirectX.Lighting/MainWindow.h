#pragma once

#include <SDL_video.h>
#include <string>

class MainWindow
{
public:
	MainWindow();
	~MainWindow();

	bool Create(std::string&& title, int width, int height);
	void Destroy();

private:
	SDL_Window* m_WindowHandle = nullptr;
};
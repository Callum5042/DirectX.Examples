#pragma once

#include <SDL_video.h>

class MainWindow
{
public:
	MainWindow();

	bool Create();

	inline SDL_Window* GetWindow() const
	{
		return m_Window;
	}

	SDL_Window* m_Window = nullptr;
};
#pragma warning (disable: 26812)
#pragma warning (disable: 6387)

#include <iostream>
#include <SDL.h>

#include "MainWindow.h"
#include "RenderDevice.h"
#include "Timer.h"

void CalculateFrameStats(Timer& m_Timer)
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((m_Timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::cout << "FPS: " << fps << " - Per frame: " << mspf << '\n';

		/*std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());*/

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

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

	Timer timer;
	timer.Reset();

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

			case SDL_KEYDOWN:
				renderDevice.OnKeyDown(e.key);
				break;

			case SDL_MOUSEMOTION:
				renderDevice.OnMouseMove(e.motion);
				break;
			}
		}
		else
		{
			timer.Tick();

			renderDevice.Render();

			CalculateFrameStats(timer);
		}
	}

	// Quit
	SDL_Quit();
	return 0;
}
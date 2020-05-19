#pragma once

#include "MainWindow.h"
#include "Events.h"
#include "Timer.h"
#include <memory>

class Engine
{
public:
	Engine();
	Engine(Engine&) = delete;
	Engine& operator=(Engine&) = delete;
	virtual ~Engine();

	int Execute(int argc, char** argv);

	static Engine* Get() { return m_Instance; }
	constexpr std::unique_ptr <Events::EventDispatcher>& GetEventDispatcher() { return m_EventDispatcher; }
	constexpr std::unique_ptr<MainWindow>& GetWindow() { return m_MainWindow; }
	constexpr Timer& GetTimer() { return m_Timer; }

	constexpr double GetFPS() { return m_FPS; }

	void Exit();

protected:
	virtual bool OnInitialise();
	virtual void OnUpdate();
	virtual void OnRender();

private:
	static Engine* m_Instance;

	bool m_Running = true;

	std::unique_ptr<MainWindow> m_MainWindow = nullptr;
	std::unique_ptr<Events::EventDispatcher> m_EventDispatcher = nullptr;
	Timer m_Timer;

	bool Initialise();

	double m_FPS = 0;
	void CalculateFrameStats();
};
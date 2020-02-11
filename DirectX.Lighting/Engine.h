#pragma once

#include "MainWindow.h"
#include "Events.h"

class Engine
{
public:
	Engine();
	virtual ~Engine();

	inline static Engine* GetInstance() { return m_Instance; }

	int Execute(int argc, char** argv);

	Events::EventDispatcher* GetEventDispatcher() const;

	void Exit();

protected:
	virtual void OnUpdate();

private:
	static Engine* m_Instance;

	bool m_Running = true;

	MainWindow* m_MainWindow = nullptr;
	Events::EventDispatcher* m_EventDispatcher = nullptr;

	bool Initialise();
};
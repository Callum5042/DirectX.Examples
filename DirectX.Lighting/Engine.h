#pragma once

#include "MainWindow.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Events.h"

class Engine
{
public:
	Engine();
	virtual ~Engine();

	inline static Engine* GetInstance() { return m_Instance; }

	int Execute(int argc, char** argv);

	Events::EventDispatcher* GetEventDispatcher() const;
	inline virtual Shader* GetShader() { return m_Shader; }
	inline RenderDevice* GetRenderDevice() { return m_RenderDevice; }

	void Exit();

protected:
	virtual bool OnInitialise();
	virtual void OnRender();
	virtual void OnUpdate();

private:
	static Engine* m_Instance;

	bool m_Running = true;

	MainWindow* m_MainWindow = nullptr;
	RenderDevice* m_RenderDevice = nullptr;
	Shader* m_Shader = nullptr;
	Events::EventDispatcher* m_EventDispatcher = nullptr;

	bool Initialise();
};
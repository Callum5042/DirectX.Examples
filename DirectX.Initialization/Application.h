#pragma once

#include "Engine.h"
#include "WindowEvents.h"
#include "Timer.h"
#include "Renderer.h"

namespace DX
{
	class Application : public Engine, public Events::WindowListener
	{
	public:
		Application() = default;

		bool OnInitialise() override;
		void OnUpdate() override;
		void OnRender() override;

		// Window Events
		void OnQuit() override;

	private:
		Renderer* m_Renderer = nullptr;
		Timer* m_Timer = nullptr;
		void CalculateFrameStats();
	};
}
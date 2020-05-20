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
		virtual ~Application();

		bool OnInitialise() override;
		void OnUpdate() override;
		void OnRender() override;

		// Window Events
		void OnQuit() override;
		void OnResize(int width, int height) override;

	private:
		Timer m_Timer;

		std::unique_ptr<DX::Renderer> m_Renderer = std::unique_ptr<Renderer>(new DX::Renderer());
		void CalculateFrameStats();
	};
}
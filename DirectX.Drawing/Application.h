#pragma once

#include "Engine.h"
#include "WindowEvents.h"
#include "InputEvents.h"

#include "Renderer.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

namespace DX
{
	class Application : public Engine, public Events::WindowListener, public Events::InputListener
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		bool OnInitialise() override;
		void OnUpdate() override;
		void OnRender() override;

		constexpr std::unique_ptr<DX::Renderer>& Renderer() { return m_Renderer; }
		constexpr std::unique_ptr<DX::Camera>& Camera() { return m_Camera; }

		// Window Events
		void OnQuit() override;
		void OnResize(int width, int height) override;

		// Input Events
		void OnKeyDown(Events::KeyData&& data) override;

	private:
		std::unique_ptr<DX::Renderer> m_Renderer = std::unique_ptr<DX::Renderer>(new DX::Renderer);
		std::unique_ptr<DX::Shader> m_Shader = std::unique_ptr<DX::Shader>(new DX::Shader);
		std::unique_ptr<DX::Camera> m_Camera = std::unique_ptr<DX::Camera>(new DX::Camera);
		std::unique_ptr<DX::Model> m_Model = std::unique_ptr<DX::Model>(new DX::Model);

		bool m_Wireframe = false;
	};
}
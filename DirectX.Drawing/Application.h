#pragma once

#include "Engine.h"
#include "WindowEvents.h"
#include "Renderer.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

namespace DX
{
	class Application : public Engine, public Events::WindowListener
	{
	public:
		Application() = default;

		bool OnInitialise() override;
		void OnUpdate() override;
		void OnRender() override;

		constexpr Renderer* Renderer() { return m_Renderer; }
		constexpr Camera* Camera() { return m_Camera; }

		// Window Events
		void OnQuit() override;
		void OnResize(int width, int height) override;

	private:
		DX::Renderer* m_Renderer = nullptr;
		DX::Shader* m_Shader = nullptr;
		DX::Camera* m_Camera = nullptr;
		DX::Model* m_Model = nullptr;
	};
}

#include "Engine.h"
#include "WindowEvents.h"
#include "InputEvents.h"
#include <iostream>

#include "Model.h"

class Application : public Engine, public Events::WindowListener
{
public:
	Application() = default;

	bool OnInitialise() override
	{
		GetShader()->CreateVertexShader("VertexShader.cso");
		GetShader()->CreatePixelShader("PixelShader.cso");

		m_Model = new Model();
		if (!m_Model->Init())
			return false;

		return true;
	}

	void OnUpdate() override
	{
		m_Model->Update();
	}

	void OnRender() override
	{
		m_Model->Render();
	}

	void OnQuit() override
	{
		Exit();
	}

private:
	Model* m_Model = nullptr;
};

int main(int argc, char** argv)
{
	Engine* engine = new Application();
	return engine->Execute(argc, argv);
}

#include "Engine.h"
#include "WindowEvents.h"
#include <iostream>

class Application : public Engine, public Events::WindowListener
{
public:
	Application() = default;

	void OnUpdate() override
	{

	}

	void OnQuit() override
	{
		Exit();
	}
};

int main(int argc, char** argv)
{
	Engine* engine = new Application();
	return engine->Execute(argc, argv);
}
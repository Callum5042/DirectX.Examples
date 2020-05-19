
#include <SDL.h>
#include "Application.h"

int main(int argc, char** argv)
{
	Engine* app = new Application();
	return app->Execute(argc, argv);
}
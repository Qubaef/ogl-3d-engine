#include "Includes.h"
#include "FirstPersonCameraController.h"
#include "Engine.h"

int main()
{
	Engine* p_engine;

	// Initialize Engine
	try
	{
		p_engine = new Engine();
	}
	catch (InitializationException &e) {
		printf("%s in %s\n", e.what(), e.get_func());
		return -1;
	}

	p_engine->run();

	return 0;
}

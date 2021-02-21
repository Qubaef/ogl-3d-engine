#include "Engine.h"

//// OpenGL initialization phase methods

void Engine::startPhaseOpenGlInit()
{
	initialize_GLFW();
	initialize_window();
	initialize_GLEW();
	setDefaultOglParameters();
}

void Engine::initialize_GLFW()
{
	// Initialise GLFW
	glewExperimental = true;												// Needed for core profile

	if (!glfwInit())
	{
		throw InitializationException("Failed to initialize GLFW.", "initialize_GLFW");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);									// 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);						// We want OpenGL 4
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);					// To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// We don't want the old OpenGL
}

void Engine::initialize_window()
{
	// Open a window and create its OpenGL context
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	EngineWindowPtr = glfwCreateWindow(constProperties.windowWidth, constProperties.windowHeight,
		constProperties.windowName, NULL, NULL);

	if (EngineWindowPtr == NULL) {
		glfwTerminate();
		throw InitializationException("Failed to open GLFW window.", "initialize_window");
	}
}

void Engine::initialize_GLEW()
{
	// Initialise GLEW
	if (EngineWindowPtr == NULL)
	{
		glfwTerminate();
		throw InitializationException("Window was not properly initialized.", "initialize_GLEW");
	}

	glfwMakeContextCurrent(EngineWindowPtr);

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw InitializationException("Failed to initialize GLEW", "initialize_GLEW");
	}
}

void Engine::setDefaultOglParameters()
{
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(EngineWindowPtr, GLFW_STICKY_KEYS, GL_TRUE);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Disable Vsync (enabling requires time synchronization)
	glfwSwapInterval(0);

	// Cull triangles which normal is not towards the camera (should be turned on for optimization)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Set default background color (will be replaced by skybox probably)
	glClearColor(0.2f, 0.8f, 1.0f, 0.0f);
}


//// Engine preparation phase methods

void Engine::startPhaseEnginePrep()
{
	//// Set default Engine properties
	setDefaultsRenderProperties();
	setDefaultsTimeProperties();

	//// TODO: Initialize task qeue

	//// TODO: Initialize workers threads

	//// TODO: Initialzie user control thread

	//// TODO: Load user data

	//// TODO: Cleanup and move the following
	
	// Initialize shaders
	p_lighting_shader = new Shader(LIGHT_SHADER_PATH_VERTEX, LIGHT_SHADER_PATH_FRAGMENT);
	p_water_shader = new Shader(WATER_SHADER_PATH_VERTEX, LIGHT_SHADER_PATH_FRAGMENT);
	// p_skybox_shader = new Shader(SKYBOX_SHADER_PATH_VERTEX, SKYBOX_SHADER_PATH_FRAGMENT);

	// Gather all shaders, which are supposed to use given light configuration
	// and pass them together as a vector to LightManager (which will properly set them up)
	std::vector<Shader*> used_shaders_vector;
	used_shaders_vector.push_back(p_lighting_shader);
	used_shaders_vector.push_back(p_water_shader);
	p_light_manager = new LightManager(used_shaders_vector);

	initialize_user_control();
	initialize_terrain();
	initialize_skybox();
}

void Engine::setDefaultsRenderProperties()
{

}

void Engine::setDefaultsTimeProperties()
{
	timeProperties.lastPrintTimestamp = glfwGetTime();
	timeProperties.lastFrameTimestamp = timeProperties.lastPrintTimestamp;
	timeProperties.printInterval = 1.0;
	timeProperties.processedFramesNumber = 0;
	timeProperties.lastFrameDelta = 0;
}

void Engine::initialize_user_control()
{
	// Initialize InputManager
	p_input_manager = new InputManager(EngineWindowPtr);

	// Initialize instance of CameraController
	// EngineControllerPtr = new FirstPersonCameraController(EngineWindowPtr, p_input_manager);
	EngineControllerPtr = new OverviewCameraController(EngineWindowPtr, p_input_manager);
}

void Engine::initialize_terrain()
{
	// p_terrain = new FlatTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_lighting_shader, EngineControllerPtr);
	// p_terrain = new RandomTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_lighting_shader, EngineControllerPtr);
	// p_terrain = new WaterTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_water_shader, EngineControllerPtr);
	// p_terrain = new DynamicTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_lighting_shader, EngineControllerPtr);
	p_terrain = new SimplexTerrainChunk(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_lighting_shader, EngineControllerPtr);

}

void Engine::initialize_skybox()
{
	p_skybox = new Skybox("skybox_1", p_skybox_shader, EngineControllerPtr);
}


//// Runtime phase methods

void Engine::saveTimestep()
{
	// Get frame timestamp
	const double currentTimestamp = glfwGetTime();

	// Calculate last frame duration and update variables
	timeProperties.lastFrameDelta = currentTimestamp - timeProperties.lastFrameTimestamp;
	timeProperties.lastFrameTimestamp = currentTimestamp;
	timeProperties.processedFramesNumber++;

	// If last printf() was more than 1 sec ago, print status and reset print timer
	if (currentTimestamp - timeProperties.lastPrintTimestamp >= 1.0) {
		printTimePerFrameInfo();
		timeProperties.processedFramesNumber = 0;
		timeProperties.lastPrintTimestamp = currentTimestamp;
	}
}

void Engine::printTimePerFrameInfo()
{
	printf("%f ms/frame : %d fps \n", 1000.0 / double(timeProperties.processedFramesNumber), timeProperties.processedFramesNumber);
}

bool Engine::checkOglErrors(const char* location)
{
	GLenum errorCode;
	bool result = false;

	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		printf_s("%s error in %s\n", error.c_str(), location);
		result = true;
	}

	return result;
}


//// Public interface methods

Engine::Engine()
{
	try
	{
		// Start OpenGL Initialization Phase
		startPhaseOpenGlInit();
	} catch (InitializationException& e) {
		fprintf(stderr, "Exception: %s\n Message: %s\n, Location: %s\n", e.get_type(), e.get_msg(), e.get_func());
		return;
	}
	
	// Start Engine Preparation Phase
	startPhaseEnginePrep();
}


RenderProperties& Engine::getRenderProperties()
{
	return renderProperties;
}


TimeProperties& Engine::getTimeProperties()
{
	return timeProperties;
}


ConstProperties& Engine::getConstProperties()
{
	return constProperties;
}


int Engine::startPhaseRuntime()
{
	// Render loop
	while (!glfwWindowShouldClose(EngineWindowPtr))
	{
		// Track time per frame value and print it's status
		saveTimestep();

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update lights configuration in the scene (if some of them are moving)
		p_light_manager->update();

		// Render terrain using shader given to it during initialization
		p_terrain->render_terrain();

		// Render skybox using its own shader
		// Render it last and use depth buffer to optimize process and skip redundant fragments
		// p_skybox->render();

		// Swap buffers after the draw (idk why, apparently it is required)
		glfwSwapBuffers(EngineWindowPtr);
		// Process pending events
		glfwPollEvents();

		// Update matrices according to user's input
		EngineControllerPtr->updateCamera();

		// Process user input
		p_input_manager->process_input();

		// check for errors
		assert(checkOglErrors("render loop") == false);
	}

	// Cleanup VBO and shader program
	glDeleteProgram(p_lighting_shader->get_ID());

	// Cleanup dynamically allocated objects
	delete p_lighting_shader;
	delete p_light_manager;
	delete EngineControllerPtr;
	delete p_terrain;
	delete p_input_manager;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}


//// Program entry-point

int main()
{
	// Entry point of the program
	Engine* enginePtr = new Engine();

	// Runtime phase
	enginePtr->startPhaseRuntime();

	return 0;
}
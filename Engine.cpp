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

	engineWindowPtr = glfwCreateWindow(constProperties.windowWidth, constProperties.windowHeight,
		constProperties.windowName, NULL, NULL);

	if (engineWindowPtr == NULL) {
		glfwTerminate();
		throw InitializationException("Failed to open GLFW window.", "initialize_window");
	}
}

void Engine::initialize_GLEW()
{
	// Initialise GLEW
	if (engineWindowPtr == NULL)
	{
		glfwTerminate();
		throw InitializationException("Window was not properly initialized.", "initialize_GLEW");
	}

	glfwMakeContextCurrent(engineWindowPtr);

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw InitializationException("Failed to initialize GLEW", "initialize_GLEW");
	}
}

void Engine::setDefaultOglParameters()
{
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(engineWindowPtr, GLFW_STICKY_KEYS, GL_TRUE);

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

	//// Attach and initialize all user-defined entities
	initializeEntityTypes();

	//// TODO: Initialize task qeue

	//// TODO: Initialize workers threads

	//// TODO: Initialzie user control thread

	// If execution mode is tests, run proper test
	if(constProperties.executionMode == ConstProperties::TESTS)
	{
		runEnginePrepTests();
		Log.PERFORMANCE("Tests completed. Continuing regular run...");
	}

	//// TODO: Cleanup and move the following
	
	// Initialize shaders
	p_lighting_shader = new Shader(LIGHT_SHADER_PATH_VERTEX, LIGHT_SHADER_PATH_FRAGMENT);
	p_water_shader = new Shader(WATER_SHADER_PATH_VERTEX, LIGHT_SHADER_PATH_FRAGMENT);
	// p_skybox_shader = new Shader(SKYBOX_SHADER_PATH_VERTEX, SKYBOX_SHADER_PATH_FRAGMENT);

	// Gather all shaders, which are supposed to use given light configuration
	// and pass them together as a vector to LightManager (which will properly set them up)
	// std::vector<Shader*> used_shaders_vector;
	// used_shaders_vector.push_back(p_lighting_shader);
	// used_shaders_vector.push_back(p_water_shader);
	// p_light_manager = new LightManager(used_shaders_vector);

	// initialize_user_control();
	// initialize_terrain();
	// initialize_skybox();
}

void Engine::runEnginePrepTests()
{
	//// SafeTaskQueue tests
	if (false)
	{
		// SafeTaskQueue test initialize:
		TestEntity testEntity;
		Task testTask(reinterpret_cast<EntityType*>(&testEntity), &EntityType::processPerFrame);
		int testSize = 100;
		for (int i = 0; i < testSize; i++)
		{
			preRenderQueue.pushTask(testTask);
		}

		// Test 1
		Log.PERFORMANCE("Test 1\n");
		auto t1 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < testSize; i++)
		{
			testEntity.processPerFrame();
		}
		auto t2 = std::chrono::high_resolution_clock::now();

		Log.PERFORMANCE("Res: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());

		// Test 1.5
		Log.PERFORMANCE("Test 1.5\n");
		t1 = std::chrono::high_resolution_clock::now();
		preRenderQueue.newFrameNotify();
		preRenderQueue.processQueue();
		t2 = std::chrono::high_resolution_clock::now();

		Log.PERFORMANCE("Res: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());

		//// Test 2
		Log.PERFORMANCE("Test 2\n");
		preRenderQueue.runWorkers(2);
		t1 = std::chrono::high_resolution_clock::now();
		preRenderQueue.newFrameNotify();
		while (!preRenderQueue.ifTaskFinished());
		t2 = std::chrono::high_resolution_clock::now();

		Log.PERFORMANCE("Res: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());

		//// Test 3
		Log.PERFORMANCE("Test 3\n");
		preRenderQueue.runWorkers(8);
		t1 = std::chrono::high_resolution_clock::now();
		preRenderQueue.newFrameNotify();
		while (!preRenderQueue.ifTaskFinished());
		t2 = std::chrono::high_resolution_clock::now();

		Log.PERFORMANCE("Res: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
	}

	//// Fopen test
	if(false)
	{
		// Fopen initialization
		FILE* pFile;
		int testSize = 1000;
		const char* filename1 = "t1";
		const char* filename2 = "t2";
		char buffer[] = "Test message\n";
		
		// Test 1
		Log.PERFORMANCE("Test 1\n");
		auto t1 = std::chrono::high_resolution_clock::now();

		pFile = fopen(filename1, "w");
		for (int i = 0; i < testSize; i++)
		{
			fwrite(buffer, sizeof(char), sizeof(buffer), pFile);
		}
		fclose(pFile);
		
		auto t2 = std::chrono::high_resolution_clock::now();
		Log.PERFORMANCE("Test result: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
		
		// Test 2
		Log.PERFORMANCE("Test 2\n");
		t1 = std::chrono::high_resolution_clock::now();
		
		for (int i = 0; i < testSize; i++)
		{
			pFile = fopen(filename1, "w");
			fwrite(buffer, sizeof(char), sizeof(buffer), pFile);
			fclose(pFile);
		}
		
		t2 = std::chrono::high_resolution_clock::now();
		Log.PERFORMANCE("Test result: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
	}
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

void Engine::initializeEntityTypes()
{
	//// User defined
	//// Attach and initialize all EntityTypes objects used in the the engine
	//// EntityType object must be added to EntitiesTypesList

	
}


void Engine::initialize_user_control()
{
	// Initialize InputManager
	p_input_manager = new InputManager(engineWindowPtr);

	// Initialize instance of CameraController
	// engineControllerPtr = new FirstPersonCameraController(engineWindowPtr, p_input_manager);
	engineControllerPtr = new OverviewCameraController(engineWindowPtr, p_input_manager);
}

void Engine::initialize_terrain()
{
	// p_terrain = new FlatTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_lighting_shader, engineControllerPtr);
	// p_terrain = new RandomTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_lighting_shader, engineControllerPtr);
	// p_terrain = new WaterTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_water_shader, engineControllerPtr);
	// p_terrain = new DynamicTerrain(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_lighting_shader, engineControllerPtr);
	p_terrain = new SimplexTerrainChunk(0, 0, SECTOR_SIZE, SECTOR_DENSITY, p_lighting_shader, engineControllerPtr);

}

void Engine::initialize_skybox()
{
	p_skybox = new Skybox("skybox_1", p_skybox_shader, engineControllerPtr);
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

	// If last print was more than 1 sec ago, print status and reset print timer
	if (currentTimestamp - timeProperties.lastPrintTimestamp >= timeProperties.printInterval) {
		printTimePerFrameInfo();
		timeProperties.processedFramesNumber = 0;
		timeProperties.lastPrintTimestamp = currentTimestamp;
	}
}

void Engine::printTimePerFrameInfo()
{
	Log.PERFORMANCE("%f ms/frame : %d fps \n", 1000.0 / double(timeProperties.processedFramesNumber), timeProperties.processedFramesNumber);
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
		Log.ERROR("%s Opengl error in %s\n", error.c_str(), location);
		result = true;
	}

	return result;
}


//// Public interface methods

Engine::Engine()
{
	// Initialize Engine.Log
	switch(constProperties.executionMode)
	{
	case ConstProperties::DEBUG:
		Log.init(true, true, true, true, true);
		break;
	default:
		Log.init(true, true, true, false, true);		
	}


	try
	{
		// Start OpenGL Initialization Phase
		startPhaseOpenGlInit();
	} catch (InitializationException& e) {
		Log.ERROR( "Exception: %s\n Message: %s\n, Location: %s\n", e.get_type(), e.get_msg(), e.get_func());
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
	while (!glfwWindowShouldClose(engineWindowPtr))
	{
		// Track time per frame value and print it's status
		saveTimestep();

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: Clear queue and fill it with new sets of tasks

		// Update lights configuration in the scene (if some of them are moving)
		// p_light_manager->update();

		// Render terrain using shader given to it during initialization
		// p_terrain->render_terrain();

		// Render skybox using its own shader
		// Render it last and use depth buffer to optimize process and skip redundant fragments
		// p_skybox->render();

		// Swap buffers after the draw (idk why, apparently it is required)
		glfwSwapBuffers(engineWindowPtr);
		// Process pending events
		glfwPollEvents();

		// Update matrices according to user's input
		// engineControllerPtr->updateCamera();

		// Process user input
		// p_input_manager->process_input();

		// Check for OpenGl rendering errors
		assert(checkOglErrors("runtimeLoop") == false);
	}

	// Cleanup VBO and shader program
	glDeleteProgram(p_lighting_shader->get_ID());

	// Cleanup dynamically allocated objects
	delete p_lighting_shader;
	delete engineControllerPtr;
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
#include <iostream>

#include "Engine.h"
#include "Exceptions/InitializationException.h"
#include "../App/Renderables/TestEntity.h"


/* OpenGL initialization phase methods */

void Engine::startPhaseOpenglInit()
{
	ZoneScoped;

	initialize_GLFW();
	initialize_window();
	initialize_GLEW();

	TracyGpuContext;

	setDefaultOglParameters();

	LOG.INFO("OGL version: %s\n", glGetString(GL_VERSION));

	int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	LOG.INFO("Max global (total) compute work group counts x:%i y:%i z:%i\n",
		work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	LOG.INFO("max local (in one shader) compute work group sizes x:%i y:%i z:%i\n",
		work_grp_size[0], work_grp_size[1], work_grp_size[2]);

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	LOG.INFO("max local work group invocations %i\n", work_grp_inv);

	LOG.INFO("max global (total) compute work group counts x:%i y:%i z:%i\n",
		work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
}

void Engine::initialize_GLFW()
{
	ZoneScoped;

	// Initialise GLFW
	glewExperimental = true;												// Needed for core profile

	if (!glfwInit())
	{
		throw InitializationException("Failed to initialize GLFW.", "initialize_GLFW");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);									// 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);						// We want OpenGL 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);					// Enable debug context
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);					// To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// We don't want the old OpenGL
}

void Engine::initialize_window()
{
	ZoneScoped;

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
	ZoneScoped;

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
	ZoneScoped;

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(engineWindowPtr, GLFW_STICKY_KEYS, GL_TRUE);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Disable Vsync (enabling requires time synchronization)
	glfwSwapInterval(1);

	// Cull triangles which normal is not towards the camera (should be turned on for optimization)
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Set default background color (will be replaced by skybox probably)
	glClearColor(0.2f, 0.8f, 1.0f, 0.0f);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(engineWindowPtr, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}


/* Engine preparation phase methods */

void Engine::startPhaseEnginePrep()
{
	ZoneScoped;

	//// Set default Engine properties
	setDefaultsRenderProperties();
	setDefaultsTimeProperties();

	// Initialize ShaderManager and insert all used shaders to it
	shaderManagerPtr = new ShaderManager(this);

	//// Initialize worker threads in preRenderQueue
	processableQueue = new ProcessableQueue(2);
}

//void Engine::runEnginePrepTests()
//{
//	ZoneScoped;
//
//	//// SafeTaskQueue tests
//	if (false)
//	{
//		// SafeTaskQueue test initialize:
//		TestEntity testEntity(this);
//		Task testTask(reinterpret_cast<IProcessable*>(&testEntity), &IProcessable::process);
//		int testSize = 100;
//		for (int i = 0; i < testSize; i++)
//		{
//			preRenderQueue.pushTaskPermanently(testTask);
//		}
//
//		// Test 1
//		INFO.PERFORMANCE("Test 1\n");
//		auto t1 = std::chrono::high_resolution_clock::now();
//		for (int i = 0; i < testSize; i++)
//		{
//			testEntity.process();
//		}
//		auto t2 = std::chrono::high_resolution_clock::now();
//
//		INFO.PERFORMANCE("Res: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
//
//		// Test 1.5
//		INFO.PERFORMANCE("Test 1.5\n");
//		t1 = std::chrono::high_resolution_clock::now();
//		preRenderQueue.refreshCycle();
//		preRenderQueue.processQueue();
//		t2 = std::chrono::high_resolution_clock::now();
//
//		INFO.PERFORMANCE("Res: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
//
//		//// Test 2
//		INFO.PERFORMANCE("Test 2\n");
//		preRenderQueue.runWorkers(2);
//		t1 = std::chrono::high_resolution_clock::now();
//		preRenderQueue.refreshCycle();
//		while (!preRenderQueue.ifFinished());
//		t2 = std::chrono::high_resolution_clock::now();
//
//		INFO.PERFORMANCE("Res: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
//
//		//// Test 3
//		INFO.PERFORMANCE("Test 3\n");
//		preRenderQueue.runWorkers(8);
//		t1 = std::chrono::high_resolution_clock::now();
//		preRenderQueue.refreshCycle();
//		while (!preRenderQueue.ifFinished());
//		t2 = std::chrono::high_resolution_clock::now();
//
//		INFO.PERFORMANCE("Res: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
//	}
//
//	//// Fopen test
//	if (false)
//	{
//		// Fopen initialization
//		FILE* pFile;
//		int testSize = 1000;
//		const char* filename1 = "t1";
//		const char* filename2 = "t2";
//		char buffer[] = "Test message\n";
//
//		// Test 1
//		INFO.PERFORMANCE("Test 1\n");
//		auto t1 = std::chrono::high_resolution_clock::now();
//
//		pFile = fopen(filename1, "w");
//		for (int i = 0; i < testSize; i++)
//		{
//			fwrite(buffer, sizeof(char), sizeof(buffer), pFile);
//		}
//		fclose(pFile);
//
//		auto t2 = std::chrono::high_resolution_clock::now();
//		INFO.PERFORMANCE("Test result: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
//
//		// Test 2
//		INFO.PERFORMANCE("Test 2\n");
//		t1 = std::chrono::high_resolution_clock::now();
//
//		for (int i = 0; i < testSize; i++)
//		{
//			pFile = fopen(filename1, "w");
//			fwrite(buffer, sizeof(char), sizeof(buffer), pFile);
//			fclose(pFile);
//		}
//
//		t2 = std::chrono::high_resolution_clock::now();
//		INFO.PERFORMANCE("Test result: %lld mic\n", std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
//	}
//}

void Engine::setDefaultsRenderProperties()
{
	ZoneScoped;
}

void Engine::setDefaultsTimeProperties()
{
	ZoneScoped;

	timeProperties.lastPrintTimestamp = glfwGetTime();
	timeProperties.lastFrameTimestamp = timeProperties.lastPrintTimestamp;
	timeProperties.printInterval = 1.0;
	timeProperties.processedFramesNumber = 0;
	timeProperties.lastFrameDelta = 0;
}


/* Runtime phase methods */

bool Engine::checkOglErrors(const char* location)
{
	ZoneScoped;

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
		LOG.ERROR("%s Opengl error in %s\n", error.c_str(), location);
 		result = true;
	}

	return result;
}


/* Public interface methods */

Engine::Engine()
{
	ZoneScoped;

	// Initialize Engine.Log
	LOG.init(true, true, true, true, true);

	try
	{
		// Start OpenGL Initialization Phase
		startPhaseOpenglInit();
	}
	catch (InitializationException& e) {
		LOG.ERROR("Exception: %s\n Message: %s\n, Location: %s\n", e.get_type(), e.get_msg(), e.get_func());
		return;
	}

	// Start Engine Preparation Phase
	startPhaseEnginePrep();
}


GLFWwindow* Engine::getGlWindow()
{
	ZoneScoped;

	return this->engineWindowPtr;
}


RenderProperties& Engine::getRenderProperties()
{
	ZoneScoped;

	return renderProperties;
}


TimeProperties& Engine::getTimeProperties()
{
	ZoneScoped;

	return timeProperties;
}


ConstProperties& Engine::getConstProperties()
{
	ZoneScoped;

	return constProperties;
}


void Engine::registerShader(Shader& shader)
{
	this->shaderManagerPtr->registerShader(shader);
}


Shader* Engine::getShaderByName(std::string name)
{
	return this->shaderManagerPtr->getShaderByName(name);
}

void Engine::setDirectionalLight()
{

}

bool Engine::registerProcessable(IProcessable* processablePtr)
{
	ZoneScoped;

	processableList.push_back(processablePtr);
	processableQueue->pushProcessable(processablePtr);

	return true;
}

void Engine::registerCamera(Camera* cameraPtr)
{
	this->cameraPtr = cameraPtr;
}

Camera* Engine::getCamera()
{
	return this->cameraPtr;
}


int Engine::run()
{
	ZoneScoped;

	// Perform additional checks to make sure that Engine is ready for running
	if (this->cameraPtr == NULL)
	{
		return -1;
	}

	// Render loop
	while (!glfwWindowShouldClose(engineWindowPtr))
	{
		// Dear ImGui: Start the frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Perform preprocess phase
		processableQueue->preprocess();

		// Start process phase
		processableQueue->process();

		//// Perform necessary side tasks, while waiting for processableQueue tasks to be completed

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update shaders uniforms with lights
		shaderManagerPtr->updatePerFrame();

		// Update matrices according to user's input
		cameraPtr->updatePerFrame();

		// Wait for preRenderQueue to be finished
		// while waiting, start performing processable tasks, if there are some already waiting for execution
		while (!processableQueue->ifFinished())
		{
			processableQueue->processNext();
		}

		// Render all rendering tasks from processableQueue
		processableQueue->render();

		// Dear ImGui: Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers after the rendering is finished
		glfwSwapBuffers(engineWindowPtr);

		// Tracy Profiler: Collect Gpu data
		TracyGpuCollect;

		// Process pending events
		glfwPollEvents();

		// Check for OpenGl rendering errors
		checkOglErrors("runtimeLoop");

		// Tracy Profiler: End frame mark
		FrameMark;
	}

	// Dear ImGui: Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

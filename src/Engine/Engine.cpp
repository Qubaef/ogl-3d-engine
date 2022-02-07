#include <iostream>

#include "Engine.h"

#include "Exceptions/InitializationException.h"
#include "App/Renderables/TestEntity.h"
#include "DebugLib/Log.h"

#include "Renderer/OpenGl/Init.h"
#include "Renderer/OpenGl/Validate.h"


/* OpenGL initialization phase methods */

void Engine::startPhaseRendererInit()
{
	// Initialize Glfw
	engineWindowPtr = initGlfw (
		props.consts.windowWidth,
		props.consts.windowHeight,
		props.consts.windowName,
		true
	);

	// Verify Glfw window
	if (engineWindowPtr == nullptr) {
		glfwTerminate();
		throw InitializationException("Failed to open Glfw window", __FUNCTION__);
	}

	// Initialize Glew
	if(initGlew() == false)
	{
		glfwTerminate();
		throw InitializationException("Failed to initialize Glew", __FUNCTION__);
	}


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
	setDefaultsPropertiesRender();
	setDefaultsTimeProperties();

	// Initialize ShaderManager and insert all used shaders to it
	shaderManagerPtr = new ShaderManager(*this);

	//// Initialize worker threads in preRenderQueue
	processableQueue = new ProcessableQueue(2);
}

void Engine::setDefaultsPropertiesRender()
{
	ZoneScoped;
}

void Engine::setDefaultsTimeProperties()
{
	props.time.lastPrintTimestamp = glfwGetTime();
	props.time.lastFrameTimestamp = props.time.lastPrintTimestamp;
	props.time.printInterval = 1.0;
	props.time.processedFramesNumber = 0;
	props.time.lastFrameDelta = 0;
}

/* Public interface methods */

Engine::Engine()
{
	try
	{
		// Start OpenGL Initialization Phase
		startPhaseRendererInit();
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


PropertiesRender& Engine::getRenderProps()
{
	return props.render;
}


PropertiesTime& Engine::getTimeProps()
{
	return props.time;
}


PropertiesConst& Engine::getConstProps()
{
	return props.consts;
}

MessageBus& Engine::getMessageBus()
{
	return messageBus;
}


void Engine::registerShader(Shader& shader)
{
	this->shaderManagerPtr->registerShader(shader);
}


Shader* Engine::getShaderByName(std::string name)
{
	return this->shaderManagerPtr->getShaderByName(name);
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

ShaderManager* Engine::getShaderManager()
{
	return this->shaderManagerPtr;
}


int Engine::run()
{
	ZoneScoped;

	// Perform additional checks to make sure that Engine is ready for running
	if (this->cameraPtr == NULL)
	{
		LOG.ERROR("Missing camera");
		return -1;
	}

	// Render loop
	while (!glfwWindowShouldClose(engineWindowPtr))
	{
		// Dear ImGui: Start the frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Publish message bus
		messageBus.publish();

		// Perform preprocess phase
		processableQueue->preprocess();

		// Start process phase
		processableQueue->process();

		//// Perform necessary side tasks, while waiting for processableQueue tasks to be completed

		// Clear buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update shaders uniforms with lights
		shaderManagerPtr->updatePerFrame();

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
		checkOglErrors(__FUNCTION__);

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

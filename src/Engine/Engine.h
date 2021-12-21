#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

/* Engine properties */
#include "Properties/RenderProperties.h"
#include "Properties/TimeProperties.h"
#include "Properties/ConstProperties.h"

/* Obligatory engine includes */
#include "CpuPipeline/IProcessable.h"

#include "CpuPipeline/ProcessableQueue.h"
#include "Camera/Camera.h"
#include "Shaders/ShaderManager.h"


class Engine
{
private:
	// =====================================================
	// ======================= Engine's properties
	// =====================================================
	RenderProperties renderProperties;
	TimeProperties timeProperties;
	ConstProperties constProperties;

	// =====================================================
	// ==================== Engine's mandatory variables
	// =====================================================

	// Pointer to window handle
	GLFWwindow* engineWindowPtr = NULL;

	// Container keeping references to all Processables registered to the engine
	std::vector<IProcessable*> processableList;

	// Queue storing and executing given pre render tasks every frame
	ProcessableQueue* processableQueue;

	// User-defined manager that supports lights logic - lights could be defined in various ways
	// The reference to the manager is supposed to be accessible from RenderEntityType, so share the same global lights info
	ShaderManager* shaderManagerPtr;

	// User-defined camera/input manager, which presence is obligatory
	// Camera updates are performed outside process/render queues
	Camera* cameraPtr = NULL;

	// =====================================================
	// ======================= Methods
	// =====================================================

	/* OpenGL initialization phase methods */

	// Start OpenGl Initialization Phase
	void startPhaseOpenglInit();

	// Initialize GLFW library
	void initialize_GLFW();

	// Initialize Engine Window
	void initialize_window();

	// Initialize GLEW library
	void initialize_GLEW();

	// Set OpenGL Engine parameters
	void setDefaultOglParameters();


	/* Engine preparation phase methods */

	// Start Engine Preparation Phase
	void startPhaseEnginePrep();

	// Set default values of RenderProperties
	// [Note: not a constructor, cause default value sometimes requires library call]
	void setDefaultsRenderProperties();

	// Set default values of TimeProperties
	// [Note: not a constructor, cause default value sometimes requires library call]
	void setDefaultsTimeProperties();

public:
	// =====================================================
	// ======================= Public interface
	// =====================================================

	Engine();

	// Return engine's window
	GLFWwindow* getGlWindow();

	// Return global engine's render properties
	RenderProperties& getRenderProperties();

	// Return global engine's render properties
	TimeProperties& getTimeProperties();

	// Return global engine's render properties
	ConstProperties& getConstProperties();

	// Register new Shader in the Engine
	void registerShader(Shader& shaderPtr);

	// Get Shader By Name
	Shader* getShaderByName(std::string name);

	// Register directional Light for 
	void setDirectionalLight();

	// Register new IProcessable static task in the Engine
	bool registerProcessable(IProcessable* processablePtr);

	// Register camera/input manager for the engine
	void registerCamera(Camera* cameraPtr);

	// Return engine's camera
	Camera* getCamera();

	// Check for ogl errors
	bool checkOglErrors(const char* location);

	// Start Runtime Phase
	int run();
};

#endif

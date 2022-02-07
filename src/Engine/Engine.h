#pragma once

#include "Include/Common.h"

/* Engine properties */
#include "Properties/Properties.h"

/* Obligatory engine includes */
#include "CpuPipeline/IProcessable.h"
#include "Components/Components.h"


class Engine
{
	// =====================================================
	// ================= Public interface
	// =====================================================
public:

	Engine();

	// Return engine's window
	GLFWwindow* getGlWindow();

	// Return engine's message bus
	MessageBus& getMessageBus();

	// Register new Shader in the Engine
	void registerShader(Shader& shaderPtr);

	// Get Shader By Name
	Shader* getShaderByName(std::string name);

	// Register new IProcessable static task in the Engine
	bool registerProcessable(IProcessable* processablePtr);

	// Register camera/input manager for the engine
	void registerCamera(Camera* cameraPtr);

	// Return engine's camera
	Camera* getCamera();

	// Return engine's shaderManager
	ShaderManager* getShaderManager();


	// =====================================================
	// =========== Runtime phase methods
	// =====================================================
public:
	// Start Runtime Phase
	void runPhaseRuntime();

private:
	// Verify Engine setup status to confirm it is ready for execution
	bool verifySetupStatus();


	// =====================================================
	// =============== Engine's properties
	// =====================================================
public:
	//
	// Engine's properties containing static hierarchical configuration and runtime data
	// Accessible for external use, so should be properly encapsulated to avoid unexpected modifications
	// 
	Properties& props;


	// =====================================================
	// ============== Engine's mandatory components
	// =====================================================
private:
	//
	// Engine's built-in components configurable via Api or properties
	//
	Components& comps;


	// =====================================================
	// =========== Initialization phase methods
	// =====================================================

	// Start Renderer Initialization Phase
	void runPhaseRendererInit();

	// Set OpenGL Engine parameters
	void setDefaultOglParameters();


	// =====================================================
	// =========== Preparation phase methods
	// =====================================================

	// Start Engine Preparation Phase
	void runPhaseEnginePrep();

	// Set default values of PropertiesRender
	void setDefaultsPropertiesRender();

	// Set default values of PropertiesTime
	void setDefaultsTimeProperties();
};

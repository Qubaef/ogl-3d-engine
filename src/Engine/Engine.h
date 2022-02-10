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
	~Engine() = default;

	// Register new Shader in the Engine
	void registerShader(Shader& shaderPtr);

	// Get Shader By Name
	Shader* getShaderByName(std::string name);

	// Register new IProcessable static task in the Engine
	bool registerProcessable(IProcessable* processablePtr);

	// Register camera for the engine
	void registerCamera(Camera* cameraPtr);

	// Register render pass manager for the engine
	void registerRenderPass(RenderPass* renderPassPtr);

	// =====================================================
	// =========== Runtime phase methods
	// =====================================================
public:
	// Start Runtime Phase
	void runPhaseRuntime();

private:
	// Verify Engine setup status to confirm it is ready for execution
	bool verifySetupStatus();

	// Render all render passes
	void render();


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
	Components comps;

	//
	// Getters for specific components
	//
public:
	// Return engine's window
	GLFWwindow* getGlfwWindow()
	{
		return comps.engineWindowPtr;
	}

	// Return engine's shaderGlobalData
	ShaderGlobalData& getShaderGlobalData()
	{
		return comps.shaderGlobalData;
	}

	// Return engine's shaderManager
	ShaderManager& getShaderManager()
	{
		return comps.shaderManager;
	}

	// Return engine's camera
	Camera* getCamera() const
	{
		return comps.cameraPtr;
	}

	// Return engine's message bus
	MessageBus& getMessageBus()
	{
		return comps.messageBus;
	}

	// Return engine's processable list
	const std::vector<IProcessable*>& getProcessableList() const
	{
		return comps.processableList;
	}

	// =====================================================
	// =========== Initialization phase methods
	// =====================================================

	// Start Renderer Initialization Phase
	void runPhaseRendererInit();

	// Set OpenGL Engine's parameters
	void setDefaultOglParameters();

	// Set default properties of the Engine
	void setDefaultProperties();

	// =====================================================
	// =========== Preparation phase methods
	// =====================================================

	// Start Engine Preparation Phase
	void runPhaseEnginePrep();

	// Initialize Engine's components
	void initComponents();

	// Dump environment properties after preparation
	void dumpEnvironmentSetup();
};

#pragma once

#include <vector>
#include "Include/Common.h"

/* Engine properties */
#include "Properties/PropertiesRender.h"
#include "Properties/PropertiesTime.h"
#include "Properties/PropertiesConst.h"
#include "Properties/Properties.h"

/* Obligatory engine includes */
#include "CpuPipeline/IProcessable.h"
#include "CpuPipeline/ProcessableQueue.h"

#include "Components/Camera/Camera.h"
#include "Components/Shaders/ShaderManager.h"
#include "Components/MessageBus/MessageBus.h"


class Engine
{

	// =====================================================
	// ======================= Engine's properties
	// =====================================================
public:
	Properties props;

	// =====================================================
	// ==================== Engine's mandatory variables
	// =====================================================
private:
	// Pointer to window handle
	GLFWwindow* engineWindowPtr = nullptr;

	// Container keeping references to all Processables registered to the engine
	std::vector<IProcessable*> processableList;

	// Queue storing and executing given pre render tasks every frame
	ProcessableQueue* processableQueue;

	// User-defined manager that supports lights logic - lights could be defined in various ways
	// The reference to the manager is supposed to be accessible from RenderEntityType, so share the same global lights info
	ShaderManager* shaderManagerPtr;

	// User-defined camera/input manager, which presence is obligatory
	// Camera updates are performed outside process/render queues
	Camera* cameraPtr = nullptr;

	// Message bus allowing communication of engine's components
	MessageBus messageBus;

	// =====================================================
	// ======================= Methods
	// =====================================================

	/* OpenGL initialization phase methods */

	// Start Renderer Initialization Phase
	void startPhaseRendererInit();

	// Set OpenGL Engine parameters
	void setDefaultOglParameters();


	/* Engine preparation phase methods */

	// Start Engine Preparation Phase
	void startPhaseEnginePrep();

	// Set default values of PropertiesRender
	// [Note: not a constructor, cause default value sometimes requires library call]
	void setDefaultsPropertiesRender();

	// Set default values of PropertiesTime
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
	PropertiesRender& getRenderProps();

	// Return global engine's render properties
	PropertiesTime& getTimeProps();

	// Return global engine's render properties
	PropertiesConst& getConstProps();

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

	// Start Runtime Phase
	int run();
};

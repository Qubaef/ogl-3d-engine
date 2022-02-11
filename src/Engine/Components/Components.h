#pragma once

#include <vector>

#include "Camera/Camera.h"
#include "Shaders/ShaderManager.h"
#include "Engine/CpuPipeline/ProcessableQueue.h"
#include "MessageBus/MessageBus.h"
#include "RenderPass/RenderPass.h"
#include "Shaders/ShaderGlobalData.h"

struct Components
{
	Components(Engine& engine) :
		shaderManager(engine)
	{
	}

	~Components() = default;

	// Disable assignment operator
	Components& operator=(const Components&) = delete;

	// =====================================================
	// =========== Built-in components
	// =====================================================

	//
	// Pointer to window handle
	//
	GLFWwindow* engineWindowPtr = nullptr;

	//
	// ShaderGlobalData manager
	//
	ShaderGlobalData shaderGlobalData;

	//
	// Queue to store and execute given processable tasks every frame
	//
	ProcessableQueue processableQueue;

	//
	// Message bus allowing communication of engine's components
	//
	MessageBus messageBus;

	//
	// Built-in configurable component to store shaders
	//
	ShaderManager shaderManager;

	// =====================================================
	// =========== Registrable components
	// =====================================================

	//
	// Registrable render passes
	//
	std::vector<RenderPass*> renderPassList;

	//
	// User-defined camera/input manager, which presence is obligatory for Engine's execution
	//
	Camera* cameraPtr = nullptr;

	//
	// Container keeping references to all Processables registered in the engine
	//
	std::vector<IProcessable*> processableList;
};

#pragma once

#include <vector>

#include "Camera/Camera.h"
#include "Shaders/ShaderManager.h"
#include "Engine/CpuPipeline/ProcessableQueue.h"
#include "MessageBus/MessageBus.h"

struct Components
{
	//
	// Pointer to window handle
	//
	GLFWwindow* engineWindowPtr = nullptr;

	//
	// Built-in configurable component to store shaders
	//
	ShaderManager* shaderManagerPtr = nullptr;

	//
	// User-defined camera/input manager, which presence is obligatory for Engine's execution
	//
	Camera* cameraPtr = nullptr;

	//
	// Container keeping references to all Processables registered in the engine
	//
	std::vector<IProcessable*> processableList;

	//
	// Queue to store and execute given processable tasks every frame
	//
	ProcessableQueue processableQueue;

	//
	// Message bus allowing communication of engine's components
	//
	MessageBus messageBus;
};
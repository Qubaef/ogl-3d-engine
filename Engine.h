#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "Includes.h"

// Obligatory engine includes
#include "EntityType.h"
#include "SafeTaskQueue.h"
#include "Logger.h"

class LightsManager;
#include "LightsManager.h"

// Entities
#include "TestEntity.h"

// Cameras
#include "FirstPersonCameraController.h"
#include "OverviewCameraController.h"

// SkyBox
#include "Skybox.h"

// Terrains
#include "FlatTerrain.h"
#include "RandomTerrain.h"
#include "WaterTerrain.h"
#include "DynamicTerrain.h"
#include "SimplexTerrainChunk.h"

// Managers
#include "InputManager.h"
#include "ShaderManager.h"


// Engine properties
#include "RenderProperties.h"
#include "TimeProperties.h"
#include "ConstProperties.h"


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
	// [Note: dot in separate container to optimize the references]

	// Pointer to window handle
	GLFWwindow* engineWindowPtr = NULL;

	// Main container keeping references to all EntityTypes used in the engine
	std::vector<EntityType*> entitiesTypesList;

	// Queue storing and executing given tasks every frame
	SafeTaskQueue preRenderQueue;

	// User-defined manager that supports lights logic - lights could be defined in various ways
	// The reference to the manager is supposed to be accessible from RenderEntityType, so share the same global lights info
	LightsManager* lightsManagerPtr;

	
	// TODO: change the structure of variables below
	// Pointer to CameraController
	CameraController* engineControllerPtr = NULL;

	// Pointer to Terrain
	Terrain* p_terrain = NULL;

	// Pointer to Skybox
	Skybox* p_skybox = NULL;

	// Pointer to Input manager
	InputManager* p_input_manager = NULL;

	// Shader with classic lighting
	Shader* p_lighting_shader = NULL;
	Shader* p_water_shader = NULL;
	Shader* p_skybox_shader = NULL;

	// =====================================================
	// ======================= Methods
	// =====================================================

	//// OpenGL initialization phase methods

	// Start OpenGl Initialization Phase
	void startPhaseOpenGlInit();
	
	// Initialize GLFW library
	void initialize_GLFW();

	// Initialize Engine Window
	void initialize_window();

	// Initialize GLEW library
	void initialize_GLEW();

	// Set OpenGL Engine parameters
	void setDefaultOglParameters();


	//// Engine preparation phase methods

	// Start Engine Preparation Phase
	void startPhaseEnginePrep();

	// TMP: functions for running tests
	void runEnginePrepTests();
	
	// Set default values of RenderProperties
	// [Note: not a constructor, cause default value sometimes requires library call]
	void setDefaultsRenderProperties();

	// Set default values of TimeProperties
	// [Note: not a constructor, cause default value sometimes requires library call]
	void setDefaultsTimeProperties();

	// Attach and initialize all user-defined entity types classes used in the engine
	// [User defined]
	void initializeEntityTypes();
	
	// TODO:
	// Initialize components responsible for user control
	void initialize_user_control();
	// TODO:
	// Initialize terrain
	void initialize_terrain();
	// TODO:
	// Initialize skybox
	void initialize_skybox();


	//// Runtime phase methods


	// Track time in Engine
	void saveTimestep();

	// Print message about fps and avg time per frame
	void printTimePerFrameInfo();

	// Check for ogl errors
	bool checkOglErrors(const char* location);

public:
	// =====================================================
	// ======================= Public interface
	// =====================================================
	
	Engine();

	// Publicly available Log
	Logger Log;

	// Returns global engine's render properties
	RenderProperties& getRenderProperties();

	// Returns global engine's render properties
	TimeProperties& getTimeProperties();

	// Returns global engine's render properties
	ConstProperties& getConstProperties();

	// Start Runtime Phase
	int startPhaseRuntime();
};

#endif

#pragma once
#include "Includes.h"

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
#include "LightManager.h"
#include "InputManager.h"
#include "ShaderManager.h"


// Engine properties
#include "RenderProperties.h"
#include "TimeProperties.h"


class Engine
{
private:
	// =====================================================
	// ======================= Engine's properties
	// =====================================================
	RenderProperties renderProperties;
	TimeProperties timeProperties;

	// =====================================================
	// ==================== Engine's mandatory variables
	// =====================================================
	// [Note: dot in separate container to optimize the references]

	// Pointer to window handle
	GLFWwindow* EngineWindowPtr = NULL;

	// TODO: change the structure of variables below
	// Pointer to CameraController
	CameraController* EngineControllerPtr = NULL;

	// Pointer to Terrain
	Terrain* p_terrain = NULL;

	// Pointer to Skybox
	Skybox* p_skybox = NULL;

	// Pointer to Input manager
	InputManager* p_input_manager = NULL;

	// Light manager
	LightManager* p_light_manager = NULL;

	// Shader with classic lighting
	Shader* p_lighting_shader = NULL;
	Shader* p_water_shader = NULL;
	Shader* p_skybox_shader = NULL;

	// =====================================================
	// ======================= Methods
	// =====================================================

	//// OpenGL initialization phase methods
	// TODO:
	// Initialize GLFW library
	bool initialize_GLFW();
	// TODO:
	// Initialize GLEW library
	bool initialize_GLEW();
	// TODO:
	// Initialize Engine Window
	bool initialize_window();

	
	//// Engine preparation phase methods

	// Set default values of RenderProperties
	// [Note: not a constructor, cause default value sometimes requires library call]
	void setDefaultsRenderProperties();

	// Set default values of TimeProperties
	// [Note: not a constructor, cause default value sometimes requires library call]
	void setDefaultsTimeProperties();
	
	// TODO:
	// Initialize components responsible for user control
	void initialize_user_control();
	// TODO:
	// Initialize terrain
	void initialize_terrain();
	// TODO:
	// Initialize skybox
	void initialize_skybox();
	// TODO:
	// Set OpenGL Engine parameters
	void set_OGL_parameters();


	//// Runtime phase methods

	// TODO: review
	// Track time in Engine
	void track_time();
	// TODO: review
	// Print message about fps and avg time per frame
	void print_time_info();

	// Check for ogl errors
	bool check_errors(const char* location);

public:
	// =====================================================
	// ======================= Public interface
	// =====================================================
	
	Engine();

	// Returns global engine's render properties
	RenderProperties& getRenderProperties();

	// Returns global engine's render properties
	TimeProperties& getTimeProperties();

	// Start runtime phase of the engine
	int runtimeStart();

	// TODO: remove
	// Get last frame delta_time
	double get_last_frame_time();
};

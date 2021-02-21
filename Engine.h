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


class Engine
{
private:
	//// Variables

	// Variable to determine draw mode
	// 0 - wireframe (only outline) mode
	// 1 - regular (filled) mode
	int draw_mode = 1;
	void set_draw_mode(int draw_mode);

	struct Time
	{
		// Variables to track time per frame value
		double last_time_print;
		int processed_frames;

		// Variables to track time since last frame
		double last_time_frame;
		double delta_time_frame;

		Time()
		{
			// First call of the function
			last_time_print = glfwGetTime();
			last_time_frame = last_time_print;
			processed_frames = 0;
			delta_time_frame = 0;
		}
	} time;

	// Pointer to window handle
	GLFWwindow* p_window = NULL;

	// Pointer to CameraController
	CameraController* p_controller = NULL;

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

	//// Methods

	// Initialize GLFW library
	bool initialize_GLFW();

	// Initialize GLEW library
	bool initialize_GLEW();

	// Initialize Engine Window
	bool initialize_window();

	// Initialize components responsible for user control
	void initialize_user_control();

	// Initialize terrain
	void initialize_terrain();

	// Initialize skybox
	void initialize_skybox();

	// Set OpenGL Engine parameters
	void set_OGL_parameters();


	// Track time in Engine
	void track_time();
	// Print message about fps and avg time per frame
	void print_time_info();

	// Check for ogl errors
	bool check_errors(const char* location);

public:
	Engine();

	// Start execution of the render loop
	int run();

	// Get last frame delta_time
	double get_last_frame_time();
};

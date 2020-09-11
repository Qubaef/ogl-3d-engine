#pragma once
#include "Includes.hpp"
#include "Shader.hpp"

// Cameras
#include "FirstPersonCameraController.hpp"
#include "OverviewCameraController.hpp"

// Terrains
#include "FlatTerrain.hpp"
#include "RandomTerrain.hpp"
#include "WaterTerrain.hpp"

// Managers
#include "LightManager.hpp"
#include "InputManager.hpp"


class Engine
{
private:
	//// Variables

	// Variable to determine draw mode
	// 0 - wireframe (only outline) mode
	// 1 - regular (filled) mode
	int draw_mode = 1;
	void set_draw_mode(int draw_mode);

	// Variables to track time per frame value
	double last_time;
	double current_time;
	int processed_frames = -1;

	// Pointer to window handle
	GLFWwindow* p_window = NULL;

	// Pointer to CameraController
	CameraController* p_controller = NULL;

	// Pointer to Terrain
	Terrain* p_terrain = NULL;

	// Pointer to Input manager
	InputManager* p_input_manager = NULL;

	// Light manager
	LightManager* p_light_manager = NULL;

	// Shader with classic lighting
	Shader* p_lighting_shader = NULL;
	Shader* p_water_shader = NULL;

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

	// Set OpenGL Engine parameters
	void set_OGL_parameters();

	// Track time per frame and print status to Console
	void track_time_per_frame();

	// Check for ogl errors
	bool check_errors(const char* location);

public:
	Engine();

	// Start execution of the render loop
	int run();
};

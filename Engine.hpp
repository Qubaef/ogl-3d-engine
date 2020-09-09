#pragma once
#include "Includes.hpp"
#include "FirstPersonCameraController.hpp"
#include "OverviewCameraController.hpp"
#include "Shader.hpp"
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

	// Pointer to Input manager
	InputManager* p_input_manager = NULL;

	// Light manager
	LightManager* p_light_manager = NULL;

	// Shader with classic lighting
	Shader* p_lighting_shader;

	// OGL objects
	GLuint main_VAO_id;
	GLuint vertices_VBO_id;
	unsigned indices_VBO_id;
	GLuint normals_VBO_id;

	//// Methods

	// Initialize GLFW library
	bool initialize_GLFW();

	// Initialize GLEW library
	bool initialize_GLEW();

	// Initialize Engine Window
	bool initialize_window();

	// Initialize OGL objects
	void initialize_OGL_objects();

	// Initialize components responsible for user control
	void initialize_user_control();

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

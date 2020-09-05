#pragma once
#include "Includes.hpp"
#include "CameraController.hpp"
#include "Shader.hpp"

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

	// Shader with classic lighting
	Shader* shader_lighting;

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

	// Initialize CameraController instance
	void initialize_camera_controller();

	// Set OpenGL Engine parameters
	void set_OGL_parameters();

	// Process user's input
	void process_input();

	// Track time per frame and print status to Console
	void track_time_per_frame();

	// Check for ogl errors
	bool check_errors(const char* location);

public:
	Engine();

	// Start execution of the render loop
	int run();
};

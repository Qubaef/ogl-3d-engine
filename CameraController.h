#pragma once
#include "Includes.h"
#include "InputManager.h"

class CameraController
{
protected:
	// Variables
	
	GLFWwindow* p_window;
	InputManager* p_input_manager;

	const float mouse_sensitivity = 0.8;

	float initial_horizontal_angle = radians(45.f);
	float initial_vertical_angle = radians(-20.f);

	double last_time;
	double current_time;

	enum DrawMode
	{
		FILLED,
		WIREFRAME
	} draw_mode;

	vec3 camera_position;
	vec3 camera_direction;
	vec3 camera_up;
	vec3 direction_flattened;

	mat4 Projection;				// Projection Matrix
	mat4 View;						// View Matrix
	mat4 Model;						// Model Matrix
	mat4 MVP;						// MVP matrix

	// methods
	void updateProjection();
	void updateView();
	void updateModel();
	void updateMVP();

	CameraController(GLFWwindow* p_window, InputManager* p_input_manager, float mouse_sens);
	CameraController(GLFWwindow* p_window, InputManager* p_input_manager, float mouse_sens, vec3 position, float vertical_angle, float horizontal_angle);

	void generate_matrices();
	float calculate_time();
	void toggleDrawMode();

public:

	// Each CameraController should provide it's own implementation of updateCamera method
	// updateCamera is being called every execution of render loop in the Engine
	virtual void updateCamera() = 0;

	// Process input common for all CameraControllers
	virtual void registerInput();
	void defaultInput();

	mat4* getProjectionMatrix();
	mat4* getViewMatrix();
	mat4* getModelMatrix();
	mat4* getMVPMatrix();

	vec3& getPosition();
};

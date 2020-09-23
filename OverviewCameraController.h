#pragma once
#include "Includes.h"
#include "CameraController.h"

class OverviewCameraController : public CameraController
{
	float horizontal_angle;
	float vertical_angle;
	
	void calculate_angles();
	vec3 calculate_midpoint();

public:
	// functions
	OverviewCameraController(GLFWwindow* p_window, InputManager* p_input_manager);
	OverviewCameraController(GLFWwindow* p_window, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle);
	void updateCamera() override;
};

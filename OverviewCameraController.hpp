#pragma once
#include "Includes.hpp"
#include "CameraController.hpp"

class OverviewCameraController : public CameraController
{
public:
	// functions
	OverviewCameraController(GLFWwindow* p_window, InputManager* p_input_manager);
	OverviewCameraController(GLFWwindow* p_window, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle);
	void updateCamera() override;
};

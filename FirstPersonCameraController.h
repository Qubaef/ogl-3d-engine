#pragma once
#include "Includes.h"
#include "CameraController.h"

class FirstPersonCameraController : public CameraController
{
	const float movement_speed = 1;

public:
	// methods
	FirstPersonCameraController(GLFWwindow* p_window, InputManager* p_input_manager);
	FirstPersonCameraController(GLFWwindow* p_window, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle);
	void registerInput() override;
	void updateCamera() override;
};
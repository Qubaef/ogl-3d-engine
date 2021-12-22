#pragma once

#include "../../Engine/Include/Common.h"
#include "CameraController.h"

class FirstPersonCameraController : public CameraController
{
	const float movement_speed = 0.1;

public:
	// methods
	FirstPersonCameraController(Engine* enginePtr, InputManager* p_input_manager);
	FirstPersonCameraController(Engine* enginePtr, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle);
	void registerDefaultInput() override;
	void updatePerFrame() override;
	void enable() override;
};
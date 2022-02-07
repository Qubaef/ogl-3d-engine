#pragma once

#include "Engine/Include/Common.h"
#include "CameraController.h"

using namespace glm;


class OverviewCameraController : public CameraController
{
	float horizontal_angle;
	float vertical_angle;
	
	void calculate_angles();
	vec3 calculate_midpoint();

public:
	// functions
	OverviewCameraController(Engine& engine, InputManager* p_input_manager);
	OverviewCameraController(Engine& engine, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle);
	void registerDefaultInput() override;
	void updatePerFrame() override;
	void enable() override;
};

#pragma once

#include "../../Engine/Include/Common.h"
#include "../../Engine/Camera/Camera.h"
#include "InputManager.h"

class Engine;

using namespace glm;

class CameraController : public Camera
{
protected:
	// Variables	

	GLFWwindow* windowPtr;
	InputManager* inputManagerPtr;

	const float mouse_sensitivity = 0.8;

	float initial_horizontal_angle = radians(45.f);
	float initial_vertical_angle = radians(-20.f);

	double last_time;
	double current_time;

	enum class DrawMode
	{
		FILLED,
		WIREFRAME
	} draw_mode;

	vec3 cameraUp;
	vec3 directionFlattened;

	// Methods
	void updateProjection();
	void updateView();
	void updateModel();
	void updateMVP();

	CameraController(Engine* enginePtr, InputManager* p_input_manager, float mouse_sens);
	CameraController(Engine* enginePtr, InputManager* p_input_manager, float mouse_sens, vec3 position, float vertical_angle, float horizontal_angle);

	void generate_matrices();
	float calculate_time();
	void toggleDrawMode();

public:

	// Each CameraController should provide it's own implementation of updateCamera method
	// updateCamera is being called every execution of render loop in the Engine
	void updatePerFrame() override = 0;

	// Each CameraController should provide it's own implementation of enable method
	// enable is being called every time the camera is enabled, so it should prepare environment for
	// the camera to be used (the window, cursor, etc.)
	virtual void enable() = 0;

	// Process input common for all CameraControllers
	virtual void registerDefaultInput();

	// Process camera input common for all CameraControllers
	void processDefaultInput();
};

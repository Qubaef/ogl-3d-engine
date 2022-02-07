#include "FirstPersonCameraController.h"
#include "Engine/Engine.h"

using namespace glm;

FirstPersonCameraController::FirstPersonCameraController(Engine& engine, InputManager* p_input_manager) :
	CameraController(engine, p_input_manager, 0.8f)
{
}


FirstPersonCameraController::FirstPersonCameraController(Engine& engine, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle) :
	CameraController(engine, p_input_manager, 0.8f, position, vertical_angle, horizontal_angle)
{
}


void FirstPersonCameraController::registerDefaultInput()
{
	ZoneScoped;

	CameraController::registerDefaultInput();

	inputManagerPtr->register_key_event(GLFW_KEY_W);
	inputManagerPtr->register_key_event(GLFW_KEY_S);
	inputManagerPtr->register_key_event(GLFW_KEY_A);
	inputManagerPtr->register_key_event(GLFW_KEY_D);
	inputManagerPtr->register_key_event(GLFW_KEY_E);
	inputManagerPtr->register_key_event(GLFW_KEY_SPACE);
	inputManagerPtr->register_key_event(GLFW_KEY_LEFT_SHIFT);
}


// Update matrices according to user's input
void FirstPersonCameraController::updatePerFrame()
{
	ZoneScoped;

	InputState& inputState = inputManagerPtr->get_input_state();

	float delta_time = calculate_time();

	// Get mouse position
	vec2 mouse_pos = inputManagerPtr->get_mouse_pos();

	const int windowWidth = engine.props.consts.windowWidth;
	const int windowHeight = engine.props.consts.windowHeight;

	// Reset mouse position for next frame
	inputManagerPtr->reset_mouse_pos(
		windowWidth / 2,
		windowHeight / 2);

	// Compute new orientation
	initial_horizontal_angle += mouse_sensitivity / 1000 * (static_cast<float>(windowWidth) / 2 - mouse_pos.x);
	initial_vertical_angle += mouse_sensitivity / 1000 * (static_cast<float>(windowHeight) / 2 - mouse_pos.y);

	// restrict vertical rotation
	if (initial_vertical_angle > radians(89.f))
	{
		initial_vertical_angle = radians(89.f);
	}
	else if (initial_vertical_angle < radians(-89.f))
	{
		initial_vertical_angle = radians(-89.f);
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	cameraDirection = vec3(
		cos(initial_vertical_angle) * sin(initial_horizontal_angle),
		sin(initial_vertical_angle),
		cos(initial_vertical_angle) * cos(initial_horizontal_angle)
	);

	directionFlattened = glm::normalize(cameraDirection);
	directionFlattened.y = 0;
	directionFlattened = glm::normalize(directionFlattened);

	// Right vector
	vec3 camera_right = normalize(cross(cameraDirection, cameraUp));

	// Movement vector
	vec3 movement_vector = vec3(0, 0, 0);

	// Move forward
	if (inputState.ifKeyPressed(GLFW_KEY_W) != KeyState::NOT_PRESSED) {
		movement_vector += directionFlattened * delta_time;
	}
	// Move backward
	if (inputState.ifKeyPressed(GLFW_KEY_S) != KeyState::NOT_PRESSED) {
		movement_vector -= directionFlattened * delta_time;
	}
	// Move upwards
	if (inputState.ifKeyPressed(GLFW_KEY_SPACE) != KeyState::NOT_PRESSED) {
		movement_vector += vec3(0, 1, 0) * delta_time;
	}
	// Move downwards
	if (inputState.ifKeyPressed(GLFW_KEY_LEFT_SHIFT) != KeyState::NOT_PRESSED) {
		movement_vector -= vec3(0, 1, 0) * delta_time;
	}
	// Strafe right
	if (inputState.ifKeyPressed(GLFW_KEY_D) != KeyState::NOT_PRESSED) {
		movement_vector += camera_right * delta_time;
	}
	// Strafe left
	if (inputState.ifKeyPressed(GLFW_KEY_A) != KeyState::NOT_PRESSED) {
		movement_vector -= camera_right * delta_time;
	}

	float speedMultiplier;
	// Speedup
	if (inputState.ifKeyPressed(GLFW_KEY_E) != KeyState::NOT_PRESSED) {
		speedMultiplier = 10;
	}
	else
	{
		speedMultiplier = 1;
	}

	// Update movement speed if it is more than 0
	if (movement_vector.x != 0 || movement_vector.y != 0 || movement_vector.z != 0)
	{
		cameraPosition += normalize(movement_vector) * movement_speed * speedMultiplier;
	}

	updateView();
	updateMVP();

	// Process default input
	processDefaultInput();
}

void FirstPersonCameraController::enable()
{
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	inputManagerPtr->reset_mouse_pos(
		engine.props.consts.windowWidth / 2,
		engine.props.consts.windowHeight / 2
	);

	registerDefaultInput();
}

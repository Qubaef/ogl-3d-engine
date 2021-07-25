#include "FirstPersonCameraController.h"
#include "../../Engine/Include/Constants.h"

FirstPersonCameraController::FirstPersonCameraController(Engine* enginePtr, InputManager* p_input_manager) :
	CameraController(enginePtr, p_input_manager, 0.8)
{
	ZoneScoped;

	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	registerInput();
}


FirstPersonCameraController::FirstPersonCameraController(Engine* enginePtr, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle) :
	CameraController(enginePtr, p_input_manager, 0.8, position, vertical_angle, horizontal_angle)
{
	ZoneScoped;

	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	registerInput();
}


void FirstPersonCameraController::registerInput()
{
	ZoneScoped;

	CameraController::registerInput();

	inputManagerPtr->register_key_event(GLFW_KEY_W);
	inputManagerPtr->register_key_event(GLFW_KEY_S);
	inputManagerPtr->register_key_event(GLFW_KEY_A);
	inputManagerPtr->register_key_event(GLFW_KEY_D);
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

	// TODO: replace constexprs with const properties of the engine
	// Reset mouse position for next frame
	inputManagerPtr->reset_mouse_pos(SCREEN_W / 2, SCREEN_H / 2);

	// TODO: replace constexprs with const properties of the engine
	// Compute new orientation
	initial_horizontal_angle += mouse_sensitivity / 1000 * float(SCREEN_W / 2 - mouse_pos.x);
	initial_vertical_angle += mouse_sensitivity / 1000 * float(SCREEN_H / 2 - mouse_pos.y);

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
	camera_direction = vec3(
		cos(initial_vertical_angle) * sin(initial_horizontal_angle),
		sin(initial_vertical_angle),
		cos(initial_vertical_angle) * cos(initial_horizontal_angle)
	);

	direction_flattened = glm::normalize(camera_direction);
	direction_flattened.y = 0;
	direction_flattened = glm::normalize(direction_flattened);

	// Right vector
	vec3 camera_right = normalize(cross(camera_direction, camera_up));

	// Movement vector
	vec3 movement_vector = vec3(0, 0, 0);

	// Move forward
	if (inputState.if_key_pressed(GLFW_KEY_W) != InputState::NOT_PRESSED) {
		movement_vector += direction_flattened * delta_time;
	}
	// Move backward
	if (inputState.if_key_pressed(GLFW_KEY_S) != InputState::NOT_PRESSED) {
		movement_vector -= direction_flattened * delta_time;
	}
	// Move upwards
	if (inputState.if_key_pressed(GLFW_KEY_SPACE) != InputState::NOT_PRESSED) {
		movement_vector += vec3(0, 1, 0) * delta_time;
	}
	// Move downwards
	if (inputState.if_key_pressed(GLFW_KEY_LEFT_SHIFT) != InputState::NOT_PRESSED) {
		movement_vector -= vec3(0, 1, 0) * delta_time;
	}
	// Strafe right
	if (inputState.if_key_pressed(GLFW_KEY_D) != InputState::NOT_PRESSED) {
		movement_vector += camera_right * delta_time;
	}
	// Strafe left
	if (inputState.if_key_pressed(GLFW_KEY_A) != InputState::NOT_PRESSED) {
		movement_vector -= camera_right * delta_time;
	}

	// Update movement speed if it is more than 0
	if (movement_vector.x != 0 || movement_vector.y != 0 || movement_vector.z != 0)
	{
		cameraPosition += normalize(movement_vector) * movement_speed;
	}

	// INFO.INFO("Pos: <%f, %f, %f> Dir: <%f, %f, %f>\n",
	// 	camera_position.x, camera_position.y, camera_position.z,
	// 	camera_direction.x, camera_direction.y, camera_direction.z);

	updateView();
	updateMVP();

	// Process default input
	defaultInput();
}
#include "FirstPersonCameraController.h"

FirstPersonCameraController::FirstPersonCameraController(GLFWwindow* p_window, InputManager* p_input_manager) :
	CameraController(p_window, p_input_manager, 0.8)
{
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


FirstPersonCameraController::FirstPersonCameraController(GLFWwindow* p_window, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle) :
	CameraController(p_window, p_input_manager, 0.8, position, vertical_angle, horizontal_angle)
{
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


// Update matrices according to user's input
void FirstPersonCameraController::updateCamera()
{
	float delta_time = calculate_time();

	// Get mouse position
	vec2 mouse_pos = InputManager::get_mouse_pos(p_window);

	// Reset mouse position for next frame
	glfwSetCursorPos(p_window, SCREEN_W / 2, SCREEN_H / 2);

	// Compute new orientation
	horizontal_angle += mouse_sensitivity / 1000 * float(SCREEN_W / 2 - mouse_pos.x);
	vertical_angle += mouse_sensitivity / 1000 * float(SCREEN_H / 2 - mouse_pos.y);

	// restrict vertical rotation
	if (vertical_angle > radians(89.f))
	{
		vertical_angle = radians(89.f);
	}
	else if (vertical_angle < radians(-89.f))
	{
		vertical_angle = radians(-89.f);
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	camera_direction = vec3(
		cos(vertical_angle) * sin(horizontal_angle),
		sin(vertical_angle),
		cos(vertical_angle) * cos(horizontal_angle)
	);

	direction_flattened = glm::normalize(camera_direction);
	direction_flattened.y = 0;
	direction_flattened = glm::normalize(direction_flattened);

	// Right vector
	vec3 camera_right = normalize(cross(camera_direction, camera_up));

	// Movement vector
	vec3 movement_vector = vec3(0, 0, 0);

	// Move forward
	if (glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS) {
		movement_vector += direction_flattened * delta_time;
	}
	// Move backward
	if (glfwGetKey(p_window, GLFW_KEY_S) == GLFW_PRESS) {
		movement_vector -= direction_flattened * delta_time;
	}
	// Move upwards
	if (glfwGetKey(p_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		movement_vector += vec3(0, 1, 0) * delta_time;
	}
	// Move downwards
	if (glfwGetKey(p_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		movement_vector -= vec3(0, 1, 0) * delta_time;
	}
	// Strafe right
	if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS) {
		movement_vector += camera_right * delta_time;
	}
	// Strafe left
	if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS) {
		movement_vector -= camera_right * delta_time;
	}

	// Update movement speed if it is more than 0
	if (movement_vector.x != 0 || movement_vector.y != 0 || movement_vector.z != 0)
	{
		camera_position += normalize(movement_vector) * movement_speed;
	}

	updateView();
	updateMVP();
}
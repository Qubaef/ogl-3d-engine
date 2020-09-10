#include "OverviewCameraController.hpp"

OverviewCameraController::OverviewCameraController(GLFWwindow* p_window, InputManager* p_input_manager) :
	CameraController(p_window, p_input_manager, 0.8)
{
	// Show the mouse
	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


OverviewCameraController::OverviewCameraController(GLFWwindow* p_window, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle) :
	CameraController(p_window, p_input_manager, 0.8, position, vertical_angle, horizontal_angle)
{
	// Show the mouse
	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void OverviewCameraController::updateCamera()
{
	float delta_time = calculate_time();

	//// Process left mouse input
	if (p_input_manager->if_l_mouse_pressed())
	{
		// Get mouse last position
		vec2 mouse_last_pos = p_input_manager->get_mouse_last_pos();

		// Get mouse current position
		vec2 mouse_current_pos = InputManager::get_mouse_pos(p_window);

		// Move along direction vector
		// calculate flat direction vector to prevent changing y coordinate while moving
		vec3 flat_direction = camera_direction;
		flat_direction.y = 0;
		flat_direction = normalize(flat_direction);

		float height_multiplier = camera_position.y;

		if (camera_position.y < 0)
		{
			height_multiplier = -height_multiplier;
		}

		// multiply by 'camera_position.y' to move camera more if it is higher
		camera_position += flat_direction * (mouse_sensitivity / 300 * float(mouse_current_pos.y - mouse_last_pos.y)) * height_multiplier;

		// Move along right vector
		// calculate flat_right vector, and rest same as above
		vec3 flat_right = normalize(cross(flat_direction, camera_up));
		camera_position -= flat_right * (mouse_sensitivity / 300 * float(mouse_current_pos.x - mouse_last_pos.x)) * height_multiplier;
	}


	//// Process right mouse input
	if (p_input_manager->if_r_mouse_pressed())
	{
		// Get mouse last position
		vec2 mouse_last_pos = p_input_manager->get_mouse_last_pos();

		// Get mouse current position
		vec2 mouse_current_pos = InputManager::get_mouse_pos(p_window);

		// Compute new orientation
		horizontal_angle -= mouse_sensitivity / 300 * float(mouse_current_pos.x - mouse_last_pos.x);
		vertical_angle += mouse_sensitivity / 300 * float(mouse_current_pos.y - mouse_last_pos.y);

		// restrict vertical rotation
		if (vertical_angle > radians(89.f))
		{
			vertical_angle = radians(89.f);
		}
		else if (vertical_angle < radians(-89.f))
		{
			vertical_angle = radians(-89.f);
		}

		// Calculate mid point, around which rotation will be performed
		float k;
		if (camera_direction.y != 0)
		{
			k = -(camera_position.y / camera_direction.y);
		}
		else
		{
			k = 10;
		}

		vec3 mid_point = camera_position + (camera_direction * k);
		float dist = distance(mid_point, camera_position);

		// BUG: first camera rotate causes position 'jump'
		// Position : Spherical coordinates to Cartesian coordinates conversion
		camera_position = vec3(
			mid_point.x + cos(vertical_angle) * sin(horizontal_angle) * dist,
			mid_point.y + sin(vertical_angle) * dist,
			mid_point.z + cos(vertical_angle) * cos(horizontal_angle) * dist
		);

		camera_direction = normalize(mid_point - camera_position);
	}

	if (p_input_manager->if_mouse_scroll_moved())
	{
		camera_position += camera_direction * (float)(p_input_manager->get_mouse_scroll_y_offset() * mouse_sensitivity * 2);
	}

	updateView();
	updateMVP();
}
#include "OverviewCameraController.h"

OverviewCameraController::OverviewCameraController(Engine* enginePtr, InputManager* p_input_manager) :
	CameraController(enginePtr, p_input_manager, 0.8)
{
}


OverviewCameraController::OverviewCameraController(Engine* enginePtr, InputManager* p_input_manager, vec3 position, float vertical_angle, float horizontal_angle) :
	CameraController(enginePtr, p_input_manager, 0.8, position, vertical_angle, horizontal_angle)
{
}


void OverviewCameraController::calculate_angles()
{
	ZoneScoped;

	// Convert initial angles to the ones relative to the mid point (to avoid weird jump at the start)
	vec3 mid_point = calculate_midpoint();
	float dist = distance(mid_point, cameraPosition);

	// calculate vertical angle by inverting the equation
	vertical_angle = asin((cameraPosition.y - mid_point.y) / dist);

	// calculate values for x and z horizon angles
	float horiz_z_val = (cameraPosition.z - mid_point.z) / (cos(initial_vertical_angle) * dist);
	float horiz_x_val = (cameraPosition.x - mid_point.x) / (cos(initial_vertical_angle) * dist);

	// find common angle which matches both horizon_z and horizon_x value, using atan2 function
	horizontal_angle = atan2(horiz_z_val, horiz_x_val);
}

vec3 OverviewCameraController::calculate_midpoint()
{
	ZoneScoped;

	// Calculate mid point, around which rotation will be performed
	float k;
	if (cameraDirection.y != 0)
	{
		k = -(cameraPosition.y / cameraDirection.y);
	}
	else
	{
		k = 10;
	}

	return cameraPosition + (cameraDirection * k);
}


void OverviewCameraController::registerDefaultInput()
{
	ZoneScoped;

	CameraController::registerDefaultInput();
}


void OverviewCameraController::updatePerFrame()
{
	ZoneScoped;

	InputState& inputState = inputManagerPtr->get_input_state();

	//// Process left mouse input
	if (inputState.l_mouse_pressed)
	{
		// Get mouse last position
		vec2 mouse_last_pos = inputState.mouse_pos;

		// Get mouse current position
		vec2 mouse_current_pos = inputManagerPtr->get_mouse_pos();

		// Move along direction vector
		// calculate flat direction vector to prevent changing y coordinate while moving
		vec3 flat_direction = cameraDirection;
		flat_direction.y = 0;
		flat_direction = normalize(flat_direction);

		float height_multiplier = cameraPosition.y;

		if (cameraPosition.y < 0)
		{
			height_multiplier = -height_multiplier;
		}

		// multiply by 'camera_position.y' to move camera more if it is higher
		cameraPosition += flat_direction * (mouse_sensitivity / 300 * float(mouse_current_pos.y - mouse_last_pos.y)) * height_multiplier;

		// Move along right vector
		// calculate flat_right vector, and rest same as above
		vec3 flat_right = normalize(cross(flat_direction, cameraUp));
		cameraPosition -= flat_right * (mouse_sensitivity / 300 * float(mouse_current_pos.x - mouse_last_pos.x)) * height_multiplier;
	}


	//// Process right mouse input
	if (inputState.r_mouse_pressed)
	{
		// Get mouse last position
		vec2 mouse_last_pos = inputState.mouse_pos;

		// Get mouse current position
		vec2 mouse_current_pos = inputManagerPtr->get_mouse_pos();

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

		vec3 mid_point = calculate_midpoint();
		float dist = distance(mid_point, cameraPosition);

		// Position : Spherical coordinates to Cartesian coordinates conversion
		cameraPosition = vec3(
			mid_point.x + cos(vertical_angle) * sin(horizontal_angle) * dist,
			mid_point.y + sin(vertical_angle) * dist,
			mid_point.z + cos(vertical_angle) * cos(horizontal_angle) * dist
		);

		cameraDirection = normalize(mid_point - cameraPosition);
	}

	if (inputState.ifMouseScrollMoved())
	{
		// Set height multiplier for non linear zooming
		float height_multiplier = cameraPosition.y;

		if (cameraPosition.y < 0)
		{
			height_multiplier = -height_multiplier;
		}

		if (height_multiplier < 1)
		{
			height_multiplier = 1;
		}

		cameraPosition += cameraDirection * (float)(inputState.mouse_scroll_y_offset * mouse_sensitivity / 3 * height_multiplier);
	}

	updateView();
	updateMVP();

	// Process default input
	processDefaultInput();
}

void OverviewCameraController::enable()
{
	// Show the mouse
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Calculate real angles values
	calculate_angles();
}

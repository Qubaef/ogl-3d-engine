#include "CameraController.h"

CameraController::CameraController(GLFWwindow* p_window, InputManager* p_input_manager, float mouse_sens) :
	mouse_sensitivity(mouse_sens)
{
	// set pointer to window
	this->p_window = p_window;
	this->p_input_manager = p_input_manager;

	// set initial values of camera
	this->camera_position = vec3(2, 10, 2);
	this->camera_direction = normalize(vec3(
		cos(initial_vertical_angle) * sin(initial_horizontal_angle),
		sin(initial_vertical_angle),
		cos(initial_vertical_angle) * cos(initial_horizontal_angle)));
	this->camera_up = vec3(0, 1, 0);

	generate_matrices();
	registerInput();

	// Set default DrawMode
	draw_mode = FILLED;

	// get time since of first init
	this->last_time = glfwGetTime();
}

CameraController::CameraController(GLFWwindow* p_window, InputManager* p_input_manager, float mouse_sens, vec3 position, float vertical_angle, float horizontal_angle) :
	mouse_sensitivity(mouse_sens)
{
	// set pointer to window
	this->p_window = p_window;
	this->p_input_manager = p_input_manager;

	this->initial_horizontal_angle = horizontal_angle;
	this->initial_vertical_angle = vertical_angle;

	// set initial values of camera
	this->camera_position = position;
	this->camera_direction = vec3(
		cos(vertical_angle) * sin(horizontal_angle),
		sin(vertical_angle),
		cos(vertical_angle) * cos(horizontal_angle));
	this->camera_up = vec3(0, 1, 0);

	generate_matrices();
	registerInput();

	// Set default DrawMode
	draw_mode = FILLED;

	// get time since of first init
	this->last_time = glfwGetTime();
}


void CameraController::generate_matrices()
{
	// generate all MVP matrices matrices
	updateProjection();
	updateView();
	updateModel();
	updateMVP();
}

float CameraController::calculate_time()
{
	// calculate time since last frame
	current_time = glfwGetTime();
	float delta_time = current_time - last_time;
	last_time = current_time;

	return delta_time;
}

void CameraController::toggleDrawMode()
{
	if (draw_mode == FILLED)
	{
		draw_mode = WIREFRAME;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		draw_mode = FILLED;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


void CameraController::updateProjection()
{
	// update Projection matrix
	Projection = perspective(
		radians((float)CAMERA_FOV),				// FOV
		(float)SCREEN_W / (float)SCREEN_H,		// aspect ratio
		CAMERA_NEAR_CLIPPING,					// near clipping plane
		CAMERA_FAR_CLIPPING						// far clipping pane
	);
}

void CameraController::updateView()
{
	// update View matrix
	View = glm::lookAt(
		camera_position,						// the position of your camera, in world space
		camera_position + camera_direction,		// where you want to look at, in world space
		camera_up								// up vector (0,1,0) on default
	);
}

void CameraController::updateModel()
{
	// update model matrix
	Model = mat4(1.0f);
}

void CameraController::updateMVP()
{
	// update MVP matrix
	MVP = Projection * View * Model;
}


void CameraController::registerInput()
{
	p_input_manager->register_key_event(GLFW_KEY_P);
}

void CameraController::defaultInput()
{
	InputState& inputState = p_input_manager->get_input_state();

	if (inputState.if_key_pressed(GLFW_KEY_P) == InputState::JUST_PRESSED)
	{
		toggleDrawMode();
	}
}


mat4* CameraController::getProjectionMatrix()
{
	return &Projection;
}

mat4* CameraController::getViewMatrix()
{
	return &View;
}

mat4* CameraController::getModelMatrix()
{
	return &Model;
}

mat4* CameraController::getMVPMatrix()
{
	return &MVP;
}

vec3& CameraController::getPosition()
{
	return camera_position;
}
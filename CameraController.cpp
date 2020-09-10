#include "CameraController.hpp"

CameraController::CameraController(GLFWwindow* p_window, InputManager* p_input_manager, float mouse_sens) :
	mouse_sensitivity(mouse_sens)
{
	// set pointer to window
	this->p_window = p_window;
	this->p_input_manager = p_input_manager;

	// set initial values of camera
	this->camera_position = vec3(2, 10, 2);
	this->camera_direction = normalize(vec3(
		cos(vertical_angle) * sin(horizontal_angle),
		sin(vertical_angle),
		cos(vertical_angle) * cos(horizontal_angle)));
	this->camera_up = vec3(0, 1, 0);

	generate_matrices();

	// get time since of first init
	this->last_time = glfwGetTime();
}


CameraController::CameraController(GLFWwindow* p_window, InputManager* p_input_manager, float mouse_sens, vec3 position, float vertical_angle, float horizontal_angle) :
	mouse_sensitivity(mouse_sens)
{
	// set pointer to window
	this->p_window = p_window;
	this->p_input_manager = p_input_manager;

	this->horizontal_angle = horizontal_angle;
	this->vertical_angle = vertical_angle;

	// set initial values of camera
	this->camera_position = position;
	this->camera_direction = vec3(
		cos(vertical_angle) * sin(horizontal_angle),
		sin(vertical_angle),
		cos(vertical_angle) * cos(horizontal_angle));
	this->camera_up = vec3(0, 1, 0);

	generate_matrices();

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


void CameraController::updateProjection()
{
	// update Projection matrix
	Projection = perspective(
		radians((float)CAMERA_FOV),				// FOV
		(float)SCREEN_W / (float)SCREEN_H,		// aspect ratio
		0.1f,									// near clipping plane
		1000.f									// far clipping pane
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
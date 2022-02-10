#include "CameraController.h"

#include "Engine/Engine.h"

using namespace glm;

CameraController::CameraController(Engine& engine, InputManager* p_input_manager, float mouse_sens) :
	Camera(engine), mouse_sensitivity(mouse_sens)
{
	ZoneScoped;

	// set pointer to window
	this->windowPtr = engine.getGlfwWindow();
	this->inputManagerPtr = p_input_manager;

	// set initial values of camera
	this->cameraPosition = glm::vec3(0, 0, 0);
	this->cameraDirection = glm::normalize(glm::vec3(
		cos(initial_vertical_angle) * sin(initial_horizontal_angle),
		sin(initial_vertical_angle),
		cos(initial_vertical_angle) * cos(initial_horizontal_angle)));
	this->cameraUp = glm::vec3(0, 1, 0);

	generate_matrices();
	CameraController::registerDefaultInput();

	// Set default DrawMode
	draw_mode = DrawMode::FILLED;

	// get time since of first init
	this->last_time = glfwGetTime();
}

CameraController::CameraController(Engine& engine, InputManager* p_input_manager, float mouse_sens, glm::vec3 position, float vertical_angle, float horizontal_angle) :
	Camera(engine), mouse_sensitivity(mouse_sens)
{
	ZoneScoped;

	// set pointer to window
	this->windowPtr = engine.getGlfwWindow();
	this->inputManagerPtr = p_input_manager;

	this->initial_horizontal_angle = horizontal_angle;
	this->initial_vertical_angle = vertical_angle;

	// set initial values of camera
	this->cameraPosition = position;
	this->cameraDirection = glm::vec3(
		cos(vertical_angle) * sin(horizontal_angle),
		sin(vertical_angle),
		cos(vertical_angle) * cos(horizontal_angle));
	this->cameraUp = glm::vec3(0, 1, 0);

	generate_matrices();
	CameraController::registerDefaultInput();

	// Set default DrawMode
	draw_mode = DrawMode::FILLED;

	// get time since of first init
	this->last_time = glfwGetTime();
}


void CameraController::generate_matrices()
{
	ZoneScoped;

	// generate all MVP matrices matrices
	updateProjection();
	updateView();
	updateModel();
	updateMVP();
}

float CameraController::calculate_time()
{
	ZoneScoped;

	// calculate time since last frame
	current_time = glfwGetTime();
	const float delta_time = (float) (current_time - last_time);
	last_time = current_time;

	return delta_time;
}

void CameraController::toggleDrawMode()
{
	ZoneScoped;

	if (draw_mode == DrawMode::FILLED)
	{
		draw_mode = DrawMode::WIREFRAME;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		draw_mode = DrawMode::FILLED;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


void CameraController::updateProjection()
{
	ZoneScoped;

	// update Projection matrix
	Projection = glm::perspective(
		glm::radians(static_cast<float>(engine.props.consts.cameraFov)),	// FOV
		static_cast<float>(engine.props.consts.windowWidth) /
		static_cast<float>(engine.props.consts.windowHeight),		// aspect ratio
		engine.props.consts.cameraNearClipping,						// near clipping plane
		engine.props.consts.cameraFarClipping						// far clipping pane
	);
}

void CameraController::updateView()
{
	ZoneScoped;

	// update View matrix
	View = glm::lookAt(
		cameraPosition,						// the position of your camera, in world space
		cameraPosition + cameraDirection,	// where you want to look at, in world space
		cameraUp							// up vector (0,1,0) on default
	);
}

void CameraController::updateModel()
{
	ZoneScoped;

	// update model matrix
	Model = mat4(1.0f);
}

void CameraController::updateMVP()
{
	ZoneScoped;

	// update MVP matrix
	MVP = Projection * View * Model;
}


void CameraController::registerDefaultInput()
{
	ZoneScoped;

	inputManagerPtr->register_key_event(GLFW_KEY_P);
}

void CameraController::processDefaultInput()
{
	ZoneScoped;

	inputManagerPtr->process_input();
	InputState& inputState = inputManagerPtr->get_input_state();

	if (inputState.ifKeyPressed(GLFW_KEY_P) == KeyState::JUST_PRESSED)
	{
		toggleDrawMode();
	}
}
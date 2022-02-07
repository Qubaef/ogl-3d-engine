#include "MenuUseCameraController.h"

MenuUseCameraController::MenuUseCameraController(Engine& engine, InputManager* p_input_manager) :
	CameraController(engine, p_input_manager, 0.8)
{
}

void MenuUseCameraController::registerDefaultInput()
{
	CameraController::registerDefaultInput();
}

void MenuUseCameraController::updatePerFrame()
{
    updateView();
	updateMVP();

    // Process default input
	processDefaultInput();
}

void MenuUseCameraController::enable()
{
    // Show the mouse
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

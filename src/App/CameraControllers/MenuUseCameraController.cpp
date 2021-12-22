#include "MenuUseCameraController.h"
#include "../../Engine/Include/Constants.h"

MenuUseCameraController::MenuUseCameraController(Engine* enginePtr, InputManager* p_input_manager) :
	CameraController(enginePtr, p_input_manager, 0.8)
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

#include "CameraController.hpp"

CameraController::CameraController(GLFWwindow* window)
{
	// set initial values
	this->window = window;
	this->cameraPosition = vec3(2, 1, 2);
	this->cameraDirection = vec3(1, 0, 1);
	this->cameraUp = vec3(0, 1, 0);

	// init matrices
	updateProjection();
	updateView();
	updateModel();
	updateMVP();

	this->lastTime = glfwGetTime();		// get time since of first init
}


// update matrices accodring to user's input
void CameraController::updateCamera()
{
	// calculate time since last frame
	currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, SCREEN_W / 2, SCREEN_H / 2);

	// Compute new orientation
	horizontalAngle += mouseSensitivity / 1000 * float(SCREEN_W / 2 - xpos);
	verticalAngle += mouseSensitivity / 1000 * float(SCREEN_H / 2 - ypos);

	// restrict vertical rotation
	if (verticalAngle > radians(89.f))
	{
		verticalAngle = radians(89.f);
	}
	else if (verticalAngle < radians(-89.f))
	{
		verticalAngle = radians(-89.f);
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	cameraDirection = vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	directionFlattened = glm::normalize(cameraDirection);
	directionFlattened.y = 0;
	directionFlattened = glm::normalize(directionFlattened);

	// Right vector
	cameraRight = normalize(cross(cameraDirection, cameraUp));

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition += directionFlattened * deltaTime * movementSpeed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPosition -= directionFlattened * deltaTime * movementSpeed;
	}
	// Move upwards
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraPosition += vec3(0, 1, 0) * deltaTime * movementSpeed;
	}
	// Move downwards
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraPosition -= vec3(0, 1, 0) * deltaTime * movementSpeed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition += cameraRight * deltaTime * movementSpeed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPosition -= cameraRight * deltaTime * movementSpeed;
	}

	updateView();
	updateMVP();

	lastTime = currentTime;
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
		cameraPosition,							// the position of your camera, in world space
		cameraPosition + cameraDirection,		// where you want to look at, in world space
		cameraUp								// up vector (0,1,0) on default
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
	return cameraPosition;
}

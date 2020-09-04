#pragma once
#include "Includes.hpp"

class CameraController
{
private:
	// Variables
	GLFWwindow* window;

	const float mouseSensitivity = 0.8;
	const float movementSpeed = 15;

	float horizontalAngle = 3.14f;
	float verticalAngle = 0;

	double lastTime;
	double currentTime;

	vec3 cameraPosition;
	vec3 cameraDirection;
	vec3 cameraRight;
	vec3 cameraUp;
	vec3 directionFlattened;

	mat4 Projection;				// Projection Matrix
	mat4 View;						// View Matrix
	mat4 Model;						// Model Matrix
	mat4 MVP;						// MVP matrix

	// functions
	void updateProjection();
	void updateView();
	void updateModel();
	void updateMVP();
public:
	// variables

	// functions
	CameraController(GLFWwindow* window);
	void updateCamera();

	mat4* getProjectionMatrix();
	mat4* getViewMatrix();
	mat4* getModelMatrix();
	mat4* getMVPMatrix();

	vec3& getPosition();
};
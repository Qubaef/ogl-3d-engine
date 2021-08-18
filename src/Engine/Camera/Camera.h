#pragma once

#include "../Include/Common.h"

using namespace glm;

class Engine;

class Camera
{
protected:
	Engine* enginePtr;

	vec3 cameraPosition;			// Camera Position Vector
	vec3 cameraDirection;			// Camera Position Vector

	mat4 Projection;				// Projection Matrix
	mat4 View;						// View Matrix
	mat4 Model;						// Model Matrix
	mat4 MVP;						// MVP matrix
public:
	Camera(Engine* enginePtr);

	// Update camera per frame
	virtual void updatePerFrame() = 0;

	mat4* getProjectionMatrix();
	mat4* getViewMatrix();
	mat4* getModelMatrix();
	mat4* getMVPMatrix();

	vec3 getPosition();
	vec3 getDirection();
};

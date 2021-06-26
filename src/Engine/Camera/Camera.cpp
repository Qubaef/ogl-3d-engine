#include "Camera.h"

Camera::Camera(Engine* enginePtr) :
	enginePtr(enginePtr)
{
}


mat4* Camera::getProjectionMatrix()
{
	ZoneScoped;

	return &Projection;
}

mat4* Camera::getViewMatrix()
{
	ZoneScoped;

	return &View;
}

mat4* Camera::getModelMatrix()
{
	ZoneScoped;

	return &Model;
}

mat4* Camera::getMVPMatrix()
{
	ZoneScoped;

	return &MVP;
}

vec3 Camera::getPosition()
{
	return cameraPosition;
}

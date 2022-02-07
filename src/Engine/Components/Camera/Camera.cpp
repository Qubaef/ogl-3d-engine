#include "Camera.h"

Camera::Camera(Engine& engine) :
	engine(engine)
{
}


glm::mat4* Camera::getProjectionMatrix()
{
	ZoneScoped;

	return &Projection;
}

glm::mat4* Camera::getViewMatrix()
{
	ZoneScoped;

	return &View;
}

glm::mat4* Camera::getModelMatrix()
{
	ZoneScoped;

	return &Model;
}

glm::mat4* Camera::getMVPMatrix()
{
	ZoneScoped;

	return &MVP;
}

glm::vec3 Camera::getPosition()
{
	return cameraPosition;
}

glm::vec3 Camera::getDirection()
{
	return cameraDirection;
}

void Camera::setPosition(glm::vec3 positionVec)
{
	this->cameraPosition = positionVec;
}

void Camera::setDirection(glm::vec3 directionVec)
{
	this->cameraDirection = directionVec;
}

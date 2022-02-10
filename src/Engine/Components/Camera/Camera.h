#pragma once

#include "Engine/Include/Common.h"

class Engine;

class Camera
{
protected:
	Engine& engine;

	glm::vec3 cameraPosition;			// Camera Position Vector
	glm::vec3 cameraDirection;			// Camera Position Vector

	glm::mat4 Projection;				// Projection Matrix
	glm::mat4 View;						// View Matrix
	glm::mat4 Model;					// Model Matrix
	glm::mat4 MVP;						// MVP matrix
public:
	Camera(Engine& engine);

	// Update camera per frame
	virtual void updatePerFrame() = 0;

	glm::mat4* getProjectionMatrix();
	glm::mat4* getViewMatrix();
	glm::mat4* getModelMatrix();
	glm::mat4* getMVPMatrix();

	glm::vec3 getPosition();
	glm::vec3 getDirection();

	void setPosition(glm::vec3 positionVec);
	void setDirection(glm::vec3 directionVec);
};

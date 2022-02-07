#pragma once
#include "Light.h"

class LightDirectional : public Light
{
	glm::vec3 direction;

public:
	LightDirectional();
	LightDirectional(glm::vec3 colorAmbient, glm::vec3 colorDiffuse, glm::vec3 colorSpecular, glm::vec3 direction);

	glm::vec3 getDirectionVal() const;
	glm::vec3& getDirectionRef();
	void setDirection(glm::vec3 newDirection);
};

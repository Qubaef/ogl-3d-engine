#pragma once
#include "Light.h"

class LightPoint : public Light
{
	glm::vec3 position;

	// Factors to define light's strength
	float constant;
	float linear;
	float quadratic;

public:
	LightPoint();
	LightPoint(glm::vec3 colorAmbient, glm::vec3 colorDiffuse, glm::vec3 colorSpecular, glm::vec3 position, float constant, float linear, float quadratic);

	glm::vec3 getPosition() const;
	void setPosition(glm::vec3 new_position);

	float getConstant() const;
	void setConstant(float new_constant);

	float getLinear() const;
	void setLinear(float new_linear);

	float getQuadratic() const;
	void setQuadratic(float new_quadratic);
};

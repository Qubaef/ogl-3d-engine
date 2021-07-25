#pragma once
#include "Light.h"

class LightPoint : public Light
{
	vec3 position;

	// Factors to define light's strength
	float constant;
	float linear;
	float quadratic;

public:
	LightPoint();
	LightPoint(vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular, vec3 position, float constant, float linear, float quadratic);

	vec3 getPosition() const;
	void setPosition(vec3 new_position);

	float getConstant() const;
	void setConstant(float new_constant);

	float getLinear() const;
	void setLinear(float new_linear);

	float getQuadratic() const;
	void setQuadratic(float new_quadratic);
};

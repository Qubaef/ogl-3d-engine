#pragma once
#include "Light.h"

class LightDirectional : public Light
{
	vec3 direction;

public:
	LightDirectional();
	LightDirectional(vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular, vec3 direction);

	vec3 getDirection() const;
	void setDirection(vec3 newDirection);
};

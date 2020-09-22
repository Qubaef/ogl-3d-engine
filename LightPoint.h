#pragma once
#include "Light.h"

class LightPoint : public Light
{
	vec3 position;

	// factors to define light's strength
	float constant;
	float linear;
	float quadratic;

public:
	LightPoint(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, float constant, float linear, float quadratic);

	vec3 get_position() const;
	void set_position(vec3 new_position);

	float get_constant() const;
	void set_constant(float new_constant);

	float get_linear() const;
	void set_linear(float new_linear);

	float get_quadratic() const;
	void set_quadratic(float new_quadratic);
};

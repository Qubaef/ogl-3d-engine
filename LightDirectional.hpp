#pragma once
#include "Light.hpp"

class LightDirectional : public Light
{
	vec3 direction;

public:
	LightDirectional(vec3 ambient, vec3 diffuse, vec3 specular, vec3 direction);

	vec3 get_direction() const;
	void set_direction(vec3 new_direction);
};

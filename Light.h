#pragma once
#include "Includes.h"

class Light
{
protected:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	Light(vec3 ambient, vec3 diffuse, vec3 specular);

public:
	vec3 get_ambient() const;
	vec3 get_diffuse() const;
	vec3 get_specular() const;
};

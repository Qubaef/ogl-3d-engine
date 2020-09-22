#include "Light.h"

Light::Light(vec3 ambient, vec3 diffuse, vec3 specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
}


vec3 Light::get_ambient() const
{
	return ambient;
}


vec3 Light::get_diffuse() const
{
	return diffuse;
}


vec3 Light::get_specular() const
{
	return specular;
}
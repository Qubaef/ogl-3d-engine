#include "LightDirectional.hpp"

LightDirectional::LightDirectional(vec3 ambient, vec3 diffuse, vec3 specular, vec3 direction) :
	Light(ambient, diffuse, specular), direction(direction)
{
}


vec3 LightDirectional::get_direction() const
{
	return direction;
}


void LightDirectional::set_direction(vec3 new_direction)
{
	direction = new_direction;
}

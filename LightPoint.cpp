#include "LightPoint.h"

LightPoint::LightPoint(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, float constant, float linear, float quadratic) :
	Light(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic)
{
}


vec3 LightPoint::get_position() const
{
	return position;
}

void LightPoint::set_position(vec3 new_position)
{
	position = new_position;
}


float LightPoint::get_constant() const
{
	return constant;
}

void LightPoint::set_constant(float new_constant)
{
	constant = new_constant;
}


float LightPoint::get_linear() const
{
	return linear;
}

void LightPoint::set_linear(float new_linear)
{
	linear = new_linear;
}


float LightPoint::get_quadratic() const
{
	return quadratic;
}

void LightPoint::set_quadratic(float new_quadratic)
{
	quadratic = new_quadratic;
}

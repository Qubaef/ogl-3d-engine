#include "LightDirectional.h"

LightDirectional::LightDirectional():
	Light(false, vec3(), vec3(), vec3()), direction(vec3())
{
}


LightDirectional::LightDirectional(vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular, vec3 direction) :
	Light(true, colorAmbient, colorDiffuse, colorSpecular), direction(direction)
{
}


vec3 LightDirectional::getDirection() const
{
	return direction;
}


void LightDirectional::setDirection(vec3 new_direction)
{
	direction = new_direction;
}

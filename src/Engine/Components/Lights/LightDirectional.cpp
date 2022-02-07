#include "LightDirectional.h"

using namespace glm;

LightDirectional::LightDirectional():
	Light(false, vec3(), vec3(), vec3()), direction(vec3())
{
}


LightDirectional::LightDirectional(vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular, vec3 direction) :
	Light(true, colorAmbient, colorDiffuse, colorSpecular), direction(direction)
{
}


vec3 LightDirectional::getDirectionVal() const
{
	return direction;
}

vec3& LightDirectional::getDirectionRef()
{
	return direction;
}


void LightDirectional::setDirection(vec3 new_direction)
{
	direction = new_direction;
}

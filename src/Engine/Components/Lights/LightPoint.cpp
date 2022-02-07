#include "LightPoint.h"

using namespace glm;

LightPoint::LightPoint() :
	Light(false, vec3(), vec3(), vec3()), position(vec3()), constant(0), linear(0), quadratic(0)
{
}

LightPoint::LightPoint(vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular, vec3 position, float constant, float linear, float quadratic) :
	Light(true, colorAmbient, colorDiffuse, colorSpecular), position(position), constant(constant), linear(linear), quadratic(quadratic)
{
}


vec3 LightPoint::getPosition() const
{
	return position;
}

void LightPoint::setPosition(vec3 new_position)
{
	position = new_position;
}


float LightPoint::getConstant() const
{
	return constant;
}

void LightPoint::setConstant(float new_constant)
{
	constant = new_constant;
}


float LightPoint::getLinear() const
{
	return linear;
}

void LightPoint::setLinear(float new_linear)
{
	linear = new_linear;
}


float LightPoint::getQuadratic() const
{
	return quadratic;
}

void LightPoint::setQuadratic(float new_quadratic)
{
	quadratic = new_quadratic;
}

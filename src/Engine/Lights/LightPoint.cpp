#include "LightPoint.h"

LightPoint::LightPoint() :
	Light(false, vec3(), vec3(), vec3()), position(vec3()), constant(0), linear(0), quadratic(0)
{
	ZoneScoped;
}

LightPoint::LightPoint(vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular, vec3 position, float constant, float linear, float quadratic) :
	Light(true, colorAmbient, colorDiffuse, colorSpecular), position(position), constant(constant), linear(linear), quadratic(quadratic)
{
	ZoneScoped;
}


vec3 LightPoint::getPosition() const
{
	ZoneScoped;
	return position;
}

void LightPoint::setPosition(vec3 new_position)
{
	ZoneScoped;
	position = new_position;
}


float LightPoint::getConstant() const
{
	ZoneScoped;
	return constant;
}

void LightPoint::setConstant(float new_constant)
{
	ZoneScoped;
	constant = new_constant;
}


float LightPoint::getLinear() const
{
	ZoneScoped;
	return linear;
}

void LightPoint::setLinear(float new_linear)
{
	ZoneScoped;
	linear = new_linear;
}


float LightPoint::getQuadratic() const
{
	ZoneScoped;
	return quadratic;
}

void LightPoint::setQuadratic(float new_quadratic)
{
	ZoneScoped;
	quadratic = new_quadratic;
}

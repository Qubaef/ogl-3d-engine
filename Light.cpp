#include "Light.h"

Light::Light(bool enabled, vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular) :
	enabled(enabled), colorAmbient(colorAmbient), colorDiffuse(colorDiffuse), colorSpecular(colorSpecular)
{
}


vec3 Light::getColorAmbient() const
{
	return colorAmbient;
}


vec3 Light::getColorDiffuse() const
{
	return colorDiffuse;
}


vec3 Light::getColorSpecular() const
{
	return colorSpecular;
}


bool Light::ifEnabled() const
{
	return enabled;
}


void Light::setEnabled(bool newEnabled)
{
	enabled = newEnabled;
}


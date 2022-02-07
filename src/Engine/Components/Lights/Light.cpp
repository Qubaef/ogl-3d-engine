#include "Light.h"

using namespace glm;

Light::Light(bool enabled, vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular) :
	colorAmbient(colorAmbient), colorDiffuse(colorDiffuse), colorSpecular(colorSpecular), enabled(enabled)
{
	ZoneScoped;

}


vec3 Light::getColorAmbient() const
{
	ZoneScoped;

	return colorAmbient;
}


vec3 Light::getColorDiffuse() const
{
	ZoneScoped;

	return colorDiffuse;
}


vec3 Light::getColorSpecular() const
{
	ZoneScoped;

	return colorSpecular;
}


bool Light::ifEnabled() const
{
	ZoneScoped;

	return enabled;
}


void Light::setEnabled(bool newEnabled)
{
	ZoneScoped;

	enabled = newEnabled;
}


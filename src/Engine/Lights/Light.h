#pragma once

#include "../Include/Common.h"

using namespace glm;

class Light
{
protected:
	vec3 colorAmbient;
	vec3 colorDiffuse;
	vec3 colorSpecular;

	bool enabled;

	Light(bool enabled, vec3 colorAmbient, vec3 colorDiffuse, vec3 colorSpecular);

public:
	// Fields getters
	vec3 getColorAmbient() const;
	vec3 getColorDiffuse() const;
	vec3 getColorSpecular() const;
	bool ifEnabled() const;

	// Change enabled field to given value
	void setEnabled(bool newEnabled);
};

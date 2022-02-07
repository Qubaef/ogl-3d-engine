#pragma once

#include "Engine/Include/Common.h"

class Light
{
protected:
	glm::vec3 colorAmbient;
	glm::vec3 colorDiffuse;
	glm::vec3 colorSpecular;

	bool enabled;

	Light(bool enabled, glm::vec3 colorAmbient, glm::vec3 colorDiffuse, glm::vec3 colorSpecular);

public:
	// Fields getters
	glm::vec3 getColorAmbient() const;
	glm::vec3 getColorDiffuse() const;
	glm::vec3 getColorSpecular() const;
	bool ifEnabled() const;

	// Change enabled field to given value
	void setEnabled(bool newEnabled);
};

#pragma once

#include "GuiProperty.h"
#include <glm/vec3.hpp>

struct Vec3PropertyContinuousModifier : GuiProperty
{
	float minValue;
	float maxValue;
	float defaultValue;
	glm::vec3& value;

	Vec3PropertyContinuousModifier(const char* name, float minValue, float maxValue, float defaultValue, glm::vec3& value);

	void display() override;

	void getMessage() override;

	~Vec3PropertyContinuousModifier() override = default;
};

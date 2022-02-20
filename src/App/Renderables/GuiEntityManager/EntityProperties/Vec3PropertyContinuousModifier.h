#pragma once

#include "GuiProperty.h"
#include <glm/vec3.hpp>

struct Vec3PropertyContinuousModifier : GuiProperty
{
	float minValue;
	float maxValue;
	glm::vec3& value;

	Vec3PropertyContinuousModifier(std::string name, float minValue, float maxValue, glm::vec3& value);

	void display() override;

	~Vec3PropertyContinuousModifier() override = default;
};

#pragma once
#include "GuiProperty.h"

struct FloatPropertyContinuousModifier : GuiProperty
{
	float minValue;
	float maxValue;
	float& value;

	FloatPropertyContinuousModifier(std::string name, float minValue, float maxValue, float& value);

	void display() override;

	~FloatPropertyContinuousModifier() override = default;
};

#pragma once

#include "GuiProperty.h"

struct IntPropertyContinuousSliderModifier : GuiProperty
{
	int minValue;
	int maxValue;
	int& value;

	IntPropertyContinuousSliderModifier(std::string name, int minValue, int maxValue, int& value);

	void display() override;

	~IntPropertyContinuousSliderModifier() override = default;
};

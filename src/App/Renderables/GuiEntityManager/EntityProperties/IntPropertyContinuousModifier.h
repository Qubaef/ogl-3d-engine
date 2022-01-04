#pragma once

#include "GuiProperty.h"

struct IntPropertyContinuousModifier : GuiProperty
{
	int minValue;
	int maxValue;
	int defaultValue;
	int& value;

	IntPropertyContinuousModifier(std::string name, int minValue, int maxValue, int defaultValue, int& value);

	void display() override;

	~IntPropertyContinuousModifier() override = default;
};

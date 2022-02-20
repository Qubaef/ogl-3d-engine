#pragma once

#include "GuiProperty.h"

struct IntPropertyContinuousDragModifier : GuiProperty
{
	int minValue;
	int maxValue;
	int& value;

	IntPropertyContinuousDragModifier(std::string name, int minValue, int maxValue, int& value);

	void display() override;

	~IntPropertyContinuousDragModifier() override = default;
};

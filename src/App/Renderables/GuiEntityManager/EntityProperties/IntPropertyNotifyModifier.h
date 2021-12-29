#pragma once

#include "GuiProperty.h"

struct IntPropertyNotifyModifier : GuiProperty
{
	int minValue;
	int maxValue;
	int defaultValue;
	int value;

	IntPropertyNotifyModifier(const char* name, int minValue, int maxValue, int defaultValue, int value);

	void display() override;

	void getMessage() override;

	~IntPropertyNotifyModifier() override = default;
};

﻿#pragma once

#include "GuiProperty.h"

struct IntPropertyNotifyModifier : GuiProperty
{
	const char* recipientName;
	int minValue;
	int maxValue;
	int defaultValue;
	int value;

	IntPropertyNotifyModifier(const char* recipientName, std::string name, int minValue, int maxValue, int defaultValue, int value);

	void display() override;

	~IntPropertyNotifyModifier() override = default;
};

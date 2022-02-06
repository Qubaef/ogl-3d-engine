#pragma once

#include "GuiProperty.h"

struct BoolPropertyTrueFalseSwitch : GuiProperty
{
	bool defaultValue;
	bool& value;

	BoolPropertyTrueFalseSwitch(std::string name, bool defaultValue, bool& value);

	void display() override;

	~BoolPropertyTrueFalseSwitch() override = default;
};

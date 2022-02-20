#pragma once

#include "GuiProperty.h"

struct BoolPropertyTrueFalseSwitch : GuiProperty
{
	bool& value;

	BoolPropertyTrueFalseSwitch(std::string name, bool& value);

	void display() override;

	~BoolPropertyTrueFalseSwitch() override = default;
};

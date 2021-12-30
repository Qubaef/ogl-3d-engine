#pragma once

#include "GuiProperty.h"

struct IntPropertyWatcher : public GuiProperty
{
	const int& value;

	IntPropertyWatcher(const char* name, const int& value);

	void display() override;

	~IntPropertyWatcher() override = default;
};

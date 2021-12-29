#pragma once

#include "GuiProperty.h"

struct IntPropertyWatcher : GuiProperty
{
	const int& value;

	IntPropertyWatcher(const char* name, const int& value);

	void display() override;

	void getMessage() override;

	~IntPropertyWatcher() override = default;
};

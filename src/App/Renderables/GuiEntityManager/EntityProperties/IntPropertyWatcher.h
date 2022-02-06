﻿#pragma once

#include "GuiProperty.h"

struct IntPropertyWatcher : GuiProperty
{
	const int& value;

	IntPropertyWatcher(std::string name, const int& value);

	void display() override;

	~IntPropertyWatcher() override = default;
};

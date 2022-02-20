#pragma once
#include <functional>

#include "GuiProperty.h"

struct LambdaProperty : GuiProperty
{
	std::function<void()> lambda;

	LambdaProperty(std::string name, const std::function<void()>& lambda) :
		GuiProperty(std::move(name)),
		lambda(lambda)
	{
	}

	void display() override
	{
		lambda();
	}

	~LambdaProperty() override = default;
};

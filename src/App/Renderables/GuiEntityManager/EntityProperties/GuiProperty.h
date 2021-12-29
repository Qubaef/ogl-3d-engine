#pragma once

#include "App/Renderables/GuiEntityManager/IDisplayable.h"

struct GuiProperty : IDisplayable
{
	const char* name;

	GuiProperty(const char* name)
		: name(name)
	{
	}

	void display() override = 0;

	// TODO:
	virtual void getMessage() = 0;

	virtual ~GuiProperty() = default;
};

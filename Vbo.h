#pragma once
#include "ObligatoryHeaders.h"

struct Vbo
{
	unsigned id;

	Vbo()
	{
		glGenBuffers(1, &id);
	}
};


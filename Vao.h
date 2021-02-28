#pragma once
#include "ObligatoryHeaders.h"

struct Vao
{
	unsigned id;

	Vao()
	{
		glGenVertexArrays(1, &id);
	}
};

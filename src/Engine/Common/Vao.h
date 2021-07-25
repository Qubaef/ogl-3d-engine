#pragma once
#include "../Include/Common.h"

struct Vao
{
	unsigned id;

	Vao()
	{
		glGenVertexArrays(1, &id);
	}
};

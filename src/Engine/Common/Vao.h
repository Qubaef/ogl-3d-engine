#pragma once
#include "Engine/Include/Common.h"

struct Vao
{
	unsigned id;

	Vao()
	{
		glGenVertexArrays(1, &id);
	}
};
